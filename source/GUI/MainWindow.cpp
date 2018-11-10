/* This source file is part of koostosh's chb project
see README for copyright notice */

#include "MainWindow.h"
#include "LoginForm.h"
#include "InputDialog.h"
#include "../CHBMain.h"

using namespace chb;

void MainWindow::BackgroundThread()
{
    std::string retstr;
    std::string commstr;

    Thread::Sleep(500);// wait for everything to get ready
    while(1)
    {
        m_chbMain->Update( &retstr, &commstr );

        if (retstr.empty() && commstr.empty())
            Thread::Sleep(5);

        array<Object^>^myStringArray = { gcnew String(retstr.c_str()), gcnew String(commstr.c_str()) };
        this->Invoke(this->mPD, myStringArray);
    }
}

void MainWindow::LoginFormReturn(std::string username,std::string password)
{
    m_chbMain->Input( std::string( "/login " ) + username + std::string( " " ) + password );
    this->inputtext->Focus();
}

void MainWindow::InputDialogReturn(std::string value, String^ type)
{
    if (type == "Add Friend")
    {
        if (!value.empty())
            m_chbMain->Input( std::string( "/friend " ) + value );
    }
}

void MainWindow::ProcessMethod(String^ print, String^ command)
{
    if (!String::IsNullOrEmpty(print))
    {
        if (this->scrollingCheckbox->Checked)
        {
            viewtext->AppendText(print);
            viewtext->SelectionStart = viewtext->Text->Length;
            viewtext->ScrollToCaret();
        }
        else
            viewtext->AppendText(print);
    }
    while (!String::IsNullOrEmpty(command))
    {
        int siz = command->IndexOf("\n");
        String^ sub = command->Substring(0, siz);
        if (siz != command->Length)
            command = command->Remove(0, siz + 1);
        else
            command->Remove(command->Length);

        if (sub == "Ln")
        {
            Form^ form = gcnew LoginForm;
            form->Owner = this;
            form->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
            form->Show();
        }
        else if (sub == "Cls")
        {
            viewtext->Clear();
        }
        else if (sub->Substring(0, 3) == "Ch:")
        {
            channel_label->Text = String::Concat(sub->Substring(3, sub->Length - 3), ":");
        }
        else if (sub->Substring(0, 4) == "FrAN")
        {
            FriendsListbox->Items->Remove(sub->Substring(4, sub->Length - 4));
            FriendsListbox->Items->Insert(0, sub->Substring(4, sub->Length - 4));
        }
        else if (sub->Substring(0, 4) == "FrAF")
        {
            FriendsListbox->Items->Remove(sub->Substring(4, sub->Length - 4));
            FriendsListbox->Items->Insert(FriendsListbox->Items->IndexOf(L"===Offline===") + 1, sub->Substring(4, sub->Length - 4));
        }
        else if (sub->Substring(0, 3) == "FrR")
        {
            FriendsListbox->Items->Remove(sub->Substring(3, sub->Length - 3));
        }
        else if (sub->Substring(0, 3) == "GuA")
        {
            GuildListbox->Items->Add(sub->Substring(3, sub->Length - 3));
        }
        else if (sub->Substring(0, 3) == "GuR")
        {
            GuildListbox->Items->Remove(sub->Substring(3, sub->Length - 3));
        }
        else
        {
            viewtext->AppendText("UIC!\r\n");
        }
    }

}

System::Void MainWindow::FriendAddButton_Click(System::Object^  sender, System::EventArgs^  e)
{
    if (dialog && !dialog->IsDisposed)
        return;
    dialog = gcnew InputDialog("Add Friend");
    dialog->Owner = this;
    dialog->Show();
}

MainWindow::MainWindow()
{
    m_chbMain = new CHBMain();

    ExitingProgram = false;
    InitializeComponent();
    this->FriendsListbox->Items->Add( L"===Offline===" );

    mPD = gcnew ProcessData( this, &MainWindow::ProcessMethod );
    backThread = gcnew Thread( gcnew ThreadStart( this, &MainWindow::BackgroundThread ) );
    backThread->Start();
}

System::Void MainWindow::FriendRemoveButton_Click( System::Object^ sender, System::EventArgs^ e )
{
    String^ s = FriendsListbox->SelectedItem->ToString();
    if ( String::IsNullOrEmpty( s ) )
        return;
    m_chbMain->Input( "/unfriend " + msclr::interop::marshal_as<std::string>( s ) );
    inputtext->Focus();
}

System::Void MainWindow::scrollingCheckbox_CheckedChanged( System::Object^ sender, System::EventArgs^ e )
{
    if ( scrollingCheckbox->Checked )
    {
        viewtext->SelectionStart = viewtext->Text->Length;
        viewtext->ScrollToCaret();
    }
}

System::Void MainWindow::GuildListbox_DoubleClick( System::Object^ sender, System::EventArgs^ e )
{
    m_chbMain->Input( "/w " + msclr::interop::marshal_as<std::string>( GuildListbox->SelectedItem->ToString() ) );
    inputtext->Focus();
}

System::Void MainWindow::FriendsListbox_DoubleClick( System::Object^ sender, System::EventArgs^ e )
{
    String^ s = FriendsListbox->SelectedItem->ToString();
    if ( s == L"===Offline===" )
        return;
    m_chbMain->Input( "/w " + msclr::interop::marshal_as<std::string>( s ) );
    inputtext->Focus();
}

System::Void MainWindow::inputtext_KeyPress( System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e )
{
    if ( e->KeyChar == ( char )Keys::Enter )
    {
        e->Handled = true;
        m_chbMain->Input( msclr::interop::marshal_as<std::string>( inputtext->Text ) );
        inputtext->Text = gcnew System::String( "" );
    }
}

System::Void MainWindow::inputtext_PreviewKeyDown( System::Object^ sender, System::Windows::Forms::PreviewKeyDownEventArgs^ e )
{
    if ( e->KeyCode == Keys::Enter )
        e->IsInputKey = true;
}

System::Void MainWindow::MainWindow_Activated( System::Object^ sender, System::EventArgs^ e )
{
    this->inputtext->Focus();
}

System::Void MainWindow::MainWindow_FormClosed( System::Object^ sender, System::Windows::Forms::FormClosedEventArgs^ e )
{
    ExitingProgram = true;
    backThread->Abort();
    Application::Exit();
}
