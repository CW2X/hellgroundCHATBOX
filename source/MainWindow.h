#pragma once
#include "base_defs.h"
#include <msclr\marshal_cppstd.h>

namespace chb {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
    using namespace System::Threading;

    typedef void (*mainDllUpdateType)(std::string*,std::string*);
    typedef void (*mainDllInputType)(std::string);

	public ref class MainWindow : public System::Windows::Forms::Form
	{
	public:
		MainWindow()
		{
            ExitingProgram = false;
			InitializeComponent();
            this->FriendsListbox->Items->Add(L"===Offline===");

            mainDllUpdateFunction = NULL;
            mainDllInputFunction = NULL;
			backThread = gcnew Thread(gcnew ThreadStart(this,&MainWindow::BackgroundThread));
            backThread->Start();
		}

        void LoginFormReturn(std::string username,std::string password);
    protected:
		~MainWindow()
		{
			if (components)
			{
				delete components;
			}
		}
        void print_msg();
        void BackgroundThread();
        String^ readData;
	
    private:
        System::Windows::Forms::TextBox^  viewtext;
        System::Windows::Forms::TextBox^  inputtext;

		System::ComponentModel::Container ^components;
        Thread^ backThread;
        bool ExitingProgram;
        mainDllUpdateType mainDllUpdateFunction;
        mainDllInputType mainDllInputFunction;

        System::Windows::Forms::Label^  label1;
        System::Windows::Forms::TabControl^  tabControl;
        System::Windows::Forms::TabPage^  tabPageFriends;
        System::Windows::Forms::TabPage^  tabPageGuild;
        System::Windows::Forms::Button^  buttonRemoveFriend;
        System::Windows::Forms::Button^  buttonAddFriend;
        System::Windows::Forms::ListBox^  FriendsListbox;
        System::Windows::Forms::TabPage^  tabPageSettings;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->viewtext = (gcnew System::Windows::Forms::TextBox());
            this->inputtext = (gcnew System::Windows::Forms::TextBox());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->tabControl = (gcnew System::Windows::Forms::TabControl());
            this->tabPageFriends = (gcnew System::Windows::Forms::TabPage());
            this->buttonRemoveFriend = (gcnew System::Windows::Forms::Button());
            this->buttonAddFriend = (gcnew System::Windows::Forms::Button());
            this->FriendsListbox = (gcnew System::Windows::Forms::ListBox());
            this->tabPageGuild = (gcnew System::Windows::Forms::TabPage());
            this->tabPageSettings = (gcnew System::Windows::Forms::TabPage());
            this->tabControl->SuspendLayout();
            this->tabPageFriends->SuspendLayout();
            this->SuspendLayout();
            // 
            // viewtext
            // 
            this->viewtext->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
                | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->viewtext->BackColor = System::Drawing::Color::White;
            this->viewtext->ForeColor = System::Drawing::Color::Black;
            this->viewtext->Location = System::Drawing::Point(12, 12);
            this->viewtext->Multiline = true;
            this->viewtext->Name = L"viewtext";
            this->viewtext->ReadOnly = true;
            this->viewtext->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
            this->viewtext->Size = System::Drawing::Size(475, 242);
            this->viewtext->TabIndex = 0;
            this->viewtext->TabStop = false;
            // 
            // inputtext
            // 
            this->inputtext->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->inputtext->Location = System::Drawing::Point(85, 260);
            this->inputtext->Name = L"inputtext";
            this->inputtext->Size = System::Drawing::Size(402, 20);
            this->inputtext->TabIndex = 1;
            this->inputtext->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MainWindow::inputtext_KeyPress);
            this->inputtext->PreviewKeyDown += gcnew System::Windows::Forms::PreviewKeyDownEventHandler(this, &MainWindow::inputtext_PreviewKeyDown);
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(9, 263);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(48, 13);
            this->label1->TabIndex = 3;
            this->label1->Text = L"channel:";
            this->label1->TextAlign = System::Drawing::ContentAlignment::TopRight;
            // 
            // tabControl
            // 
            this->tabControl->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->tabControl->Controls->Add(this->tabPageFriends);
            this->tabControl->Controls->Add(this->tabPageGuild);
            this->tabControl->Controls->Add(this->tabPageSettings);
            this->tabControl->Location = System::Drawing::Point(493, 12);
            this->tabControl->Name = L"tabControl";
            this->tabControl->SelectedIndex = 0;
            this->tabControl->Size = System::Drawing::Size(147, 268);
            this->tabControl->TabIndex = 2;
            this->tabControl->TabStop = false;
            // 
            // tabPageFriends
            // 
            this->tabPageFriends->Controls->Add(this->buttonRemoveFriend);
            this->tabPageFriends->Controls->Add(this->buttonAddFriend);
            this->tabPageFriends->Controls->Add(this->FriendsListbox);
            this->tabPageFriends->Location = System::Drawing::Point(4, 22);
            this->tabPageFriends->Name = L"tabPageFriends";
            this->tabPageFriends->Padding = System::Windows::Forms::Padding(3);
            this->tabPageFriends->Size = System::Drawing::Size(139, 242);
            this->tabPageFriends->TabIndex = 0;
            this->tabPageFriends->Text = L"Friends";
            this->tabPageFriends->UseVisualStyleBackColor = true;
            // 
            // buttonRemoveFriend
            // 
            this->buttonRemoveFriend->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
            this->buttonRemoveFriend->Location = System::Drawing::Point(72, 213);
            this->buttonRemoveFriend->Name = L"buttonRemoveFriend";
            this->buttonRemoveFriend->Size = System::Drawing::Size(61, 23);
            this->buttonRemoveFriend->TabIndex = 2;
            this->buttonRemoveFriend->Text = L"Remove";
            this->buttonRemoveFriend->UseVisualStyleBackColor = true;
            // 
            // buttonAddFriend
            // 
            this->buttonAddFriend->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
            this->buttonAddFriend->Location = System::Drawing::Point(6, 213);
            this->buttonAddFriend->Name = L"buttonAddFriend";
            this->buttonAddFriend->Size = System::Drawing::Size(61, 23);
            this->buttonAddFriend->TabIndex = 1;
            this->buttonAddFriend->Text = L"Add";
            this->buttonAddFriend->UseVisualStyleBackColor = true;
            // 
            // FriendsListbox
            // 
            this->FriendsListbox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
                | System::Windows::Forms::AnchorStyles::Left));
            this->FriendsListbox->FormattingEnabled = true;
            this->FriendsListbox->IntegralHeight = false;
            this->FriendsListbox->Location = System::Drawing::Point(0, 0);
            this->FriendsListbox->Name = L"FriendsListbox";
            this->FriendsListbox->Size = System::Drawing::Size(139, 207);
            this->FriendsListbox->TabIndex = 0;
            this->FriendsListbox->TabStop = false;
            // 
            // tabPageGuild
            // 
            this->tabPageGuild->Location = System::Drawing::Point(4, 22);
            this->tabPageGuild->Name = L"tabPageGuild";
            this->tabPageGuild->Padding = System::Windows::Forms::Padding(3);
            this->tabPageGuild->Size = System::Drawing::Size(139, 242);
            this->tabPageGuild->TabIndex = 1;
            this->tabPageGuild->Text = L"Guild";
            this->tabPageGuild->UseVisualStyleBackColor = true;
            // 
            // tabPageSettings
            // 
            this->tabPageSettings->Location = System::Drawing::Point(4, 22);
            this->tabPageSettings->Name = L"tabPageSettings";
            this->tabPageSettings->Padding = System::Windows::Forms::Padding(3);
            this->tabPageSettings->Size = System::Drawing::Size(139, 242);
            this->tabPageSettings->TabIndex = 2;
            this->tabPageSettings->Text = L"Settings";
            this->tabPageSettings->UseVisualStyleBackColor = true;
            // 
            // MainWindow
            // 
            this->ClientSize = System::Drawing::Size(652, 292);
            this->Controls->Add(this->tabControl);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->inputtext);
            this->Controls->Add(this->viewtext);
            this->MaximizeBox = false;
            this->Name = L"MainWindow";
            this->ShowIcon = false;
            this->Activated += gcnew System::EventHandler(this, &MainWindow::MainWindow_Activated);
            this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MainWindow::MainWindow_FormClosed);
            this->tabControl->ResumeLayout(false);
            this->tabPageFriends->ResumeLayout(false);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

    private:
        void CreateLoginForm();

        System::Void MainWindow_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e)
        {
            ExitingProgram = true;
            backThread->Abort();
            Application::Exit();
        }

        System::Void MainWindow_Activated(System::Object^  sender, System::EventArgs^  e)
        {
            this->inputtext->Focus();
        }

        System::Void inputtext_PreviewKeyDown(System::Object^  sender, System::Windows::Forms::PreviewKeyDownEventArgs^  e)
        {
            if (e->KeyCode == Keys::Enter)
                e->IsInputKey = true;
        }
        
        System::Void inputtext_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e)
        {
            if (e->KeyChar == (char)Keys::Enter)
            {
                e->Handled = true;
                if (mainDllInputFunction != NULL)
                mainDllInputFunction(msclr::interop::marshal_as<std::string>(inputtext->Text));
                inputtext->Text = gcnew System::String("");
            }
        }
};

}
