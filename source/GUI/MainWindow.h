/* This source file is part of koostosh's chb project
see README for copyright notice */

#pragma once
#include <msclr\marshal_cppstd.h>

class CHBMain;

namespace chb {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
    using namespace System::Threading;

	public ref class MainWindow : public System::Windows::Forms::Form
	{
	public:
		MainWindow();

        void LoginFormReturn(std::string username,std::string password);
        void InputDialogReturn(std::string value,String^ type);
    protected:
		~MainWindow()
		{
			if (components)
			{
				delete components;
			}
		}

	    Thread^ backThread;
        bool ExitingProgram;
        CHBMain* m_chbMain;

    private:
        void BackgroundThread();
        delegate void ProcessData(String^ print, String^ command);
        ProcessData^ mPD;
        void ProcessMethod(String^ print, String^ command);
        Form^ dialog;

        System::Windows::Forms::Button^         FriendRemoveButton;
        System::Windows::Forms::Button^         FriendAddButton;
        System::Windows::Forms::RichTextBox^    viewtext;
        System::Windows::Forms::TextBox^        inputtext;
		System::ComponentModel::Container^      components;
        System::Windows::Forms::Label^          channel_label;
        System::Windows::Forms::TabControl^     tabControl;
        System::Windows::Forms::TabPage^        tabPageFriends;
        System::Windows::Forms::TabPage^        tabPageGuild;
        System::Windows::Forms::ListBox^        FriendsListbox;
        System::Windows::Forms::ListBox^        GuildListbox;
        System::Windows::Forms::CheckBox^       scrollingCheckbox;
        System::Windows::Forms::TabPage^        tabPageSettings;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->viewtext = (gcnew System::Windows::Forms::RichTextBox());
            this->inputtext = (gcnew System::Windows::Forms::TextBox());
            this->channel_label = (gcnew System::Windows::Forms::Label());
            this->tabControl = (gcnew System::Windows::Forms::TabControl());
            this->tabPageFriends = (gcnew System::Windows::Forms::TabPage());
            this->FriendsListbox = (gcnew System::Windows::Forms::ListBox());
            this->tabPageGuild = (gcnew System::Windows::Forms::TabPage());
            this->GuildListbox = (gcnew System::Windows::Forms::ListBox());
            this->tabPageSettings = (gcnew System::Windows::Forms::TabPage());
            this->scrollingCheckbox = (gcnew System::Windows::Forms::CheckBox());
            this->FriendAddButton = (gcnew System::Windows::Forms::Button());
            this->FriendRemoveButton = (gcnew System::Windows::Forms::Button());
            this->tabControl->SuspendLayout();
            this->tabPageFriends->SuspendLayout();
            this->tabPageGuild->SuspendLayout();
            this->tabPageSettings->SuspendLayout();
            this->SuspendLayout();
            // 
            // viewtext
            // 
            this->viewtext->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
                | System::Windows::Forms::AnchorStyles::Left)
                | System::Windows::Forms::AnchorStyles::Right));
            this->viewtext->BackColor = System::Drawing::Color::White;
            this->viewtext->DetectUrls = false;
            this->viewtext->ForeColor = System::Drawing::Color::Black;
            this->viewtext->Location = System::Drawing::Point(12, 12);
            this->viewtext->Name = L"viewtext";
            this->viewtext->ReadOnly = true;
            this->viewtext->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::Vertical;
            this->viewtext->Size = System::Drawing::Size(475, 242);
            this->viewtext->TabIndex = 0;
            this->viewtext->TabStop = false;
            this->viewtext->Text = L"";
            // 
            // inputtext
            // 
            this->inputtext->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
                | System::Windows::Forms::AnchorStyles::Right));
            this->inputtext->Location = System::Drawing::Point(115, 260);
            this->inputtext->Name = L"inputtext";
            this->inputtext->Size = System::Drawing::Size(372, 20);
            this->inputtext->TabIndex = 1;
            this->inputtext->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MainWindow::inputtext_KeyPress);
            this->inputtext->PreviewKeyDown += gcnew System::Windows::Forms::PreviewKeyDownEventHandler(this, &MainWindow::inputtext_PreviewKeyDown);
            // 
            // channel_label
            // 
            this->channel_label->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
            this->channel_label->AutoSize = true;
            this->channel_label->Location = System::Drawing::Point(9, 263);
            this->channel_label->Name = L"channel_label";
            this->channel_label->Size = System::Drawing::Size(48, 13);
            this->channel_label->TabIndex = 3;
            this->channel_label->Text = L"channel:";
            this->channel_label->TextAlign = System::Drawing::ContentAlignment::TopRight;
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
            this->tabPageFriends->Controls->Add(this->FriendRemoveButton);
            this->tabPageFriends->Controls->Add(this->FriendAddButton);
            this->tabPageFriends->Controls->Add(this->FriendsListbox);
            this->tabPageFriends->Location = System::Drawing::Point(4, 22);
            this->tabPageFriends->Name = L"tabPageFriends";
            this->tabPageFriends->Padding = System::Windows::Forms::Padding(3);
            this->tabPageFriends->Size = System::Drawing::Size(139, 242);
            this->tabPageFriends->TabIndex = 0;
            this->tabPageFriends->Text = L"Friends";
            this->tabPageFriends->UseVisualStyleBackColor = true;
            // 
            // FriendsListbox
            // 
            this->FriendsListbox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
                | System::Windows::Forms::AnchorStyles::Left));
            this->FriendsListbox->FormattingEnabled = true;
            this->FriendsListbox->IntegralHeight = false;
            this->FriendsListbox->Location = System::Drawing::Point(0, 0);
            this->FriendsListbox->Name = L"FriendsListbox";
            this->FriendsListbox->Size = System::Drawing::Size(139, 213);
            this->FriendsListbox->TabIndex = 0;
            this->FriendsListbox->TabStop = false;
            this->FriendsListbox->DoubleClick += gcnew System::EventHandler(this, &MainWindow::FriendsListbox_DoubleClick);
            // 
            // tabPageGuild
            // 
            this->tabPageGuild->Controls->Add(this->GuildListbox);
            this->tabPageGuild->Location = System::Drawing::Point(4, 22);
            this->tabPageGuild->Name = L"tabPageGuild";
            this->tabPageGuild->Padding = System::Windows::Forms::Padding(3);
            this->tabPageGuild->Size = System::Drawing::Size(139, 242);
            this->tabPageGuild->TabIndex = 1;
            this->tabPageGuild->Text = L"Guild";
            this->tabPageGuild->UseVisualStyleBackColor = true;
            // 
            // GuildListbox
            // 
            this->GuildListbox->FormattingEnabled = true;
            this->GuildListbox->IntegralHeight = false;
            this->GuildListbox->Location = System::Drawing::Point(0, 0);
            this->GuildListbox->Name = L"GuildListbox";
            this->GuildListbox->Size = System::Drawing::Size(139, 242);
            this->GuildListbox->TabIndex = 0;
            this->GuildListbox->DoubleClick += gcnew System::EventHandler(this, &MainWindow::GuildListbox_DoubleClick);
            // 
            // tabPageSettings
            // 
            this->tabPageSettings->Controls->Add(this->scrollingCheckbox);
            this->tabPageSettings->Location = System::Drawing::Point(4, 22);
            this->tabPageSettings->Name = L"tabPageSettings";
            this->tabPageSettings->Padding = System::Windows::Forms::Padding(3);
            this->tabPageSettings->Size = System::Drawing::Size(139, 242);
            this->tabPageSettings->TabIndex = 2;
            this->tabPageSettings->Text = L"Settings";
            this->tabPageSettings->UseVisualStyleBackColor = true;
            // 
            // scrollingCheckbox
            // 
            this->scrollingCheckbox->AutoSize = true;
            this->scrollingCheckbox->Checked = true;
            this->scrollingCheckbox->CheckState = System::Windows::Forms::CheckState::Checked;
            this->scrollingCheckbox->Location = System::Drawing::Point(6, 3);
            this->scrollingCheckbox->Name = L"scrollingCheckbox";
            this->scrollingCheckbox->Size = System::Drawing::Size(100, 17);
            this->scrollingCheckbox->TabIndex = 0;
            this->scrollingCheckbox->Text = L"Enable scrolling";
            this->scrollingCheckbox->UseVisualStyleBackColor = true;
            this->scrollingCheckbox->CheckedChanged += gcnew System::EventHandler(this, &MainWindow::scrollingCheckbox_CheckedChanged);
            // 
            // FriendAddButton
            // 
            this->FriendAddButton->Location = System::Drawing::Point(3, 216);
            this->FriendAddButton->Name = L"FriendAddButton";
            this->FriendAddButton->Size = System::Drawing::Size(63, 23);
            this->FriendAddButton->TabIndex = 1;
            this->FriendAddButton->Text = L"Add";
            this->FriendAddButton->UseVisualStyleBackColor = true;
            this->FriendAddButton->Click += gcnew System::EventHandler(this, &MainWindow::FriendAddButton_Click);
            // 
            // FriendRemoveButton
            // 
            this->FriendRemoveButton->Location = System::Drawing::Point(73, 216);
            this->FriendRemoveButton->Name = L"FriendRemoveButton";
            this->FriendRemoveButton->Size = System::Drawing::Size(63, 23);
            this->FriendRemoveButton->TabIndex = 2;
            this->FriendRemoveButton->Text = L"Remove";
            this->FriendRemoveButton->UseVisualStyleBackColor = true;
            this->FriendRemoveButton->Click += gcnew System::EventHandler(this, &MainWindow::FriendRemoveButton_Click);
            // 
            // MainWindow
            // 
            this->ClientSize = System::Drawing::Size(652, 292);
            this->Controls->Add(this->tabControl);
            this->Controls->Add(this->channel_label);
            this->Controls->Add(this->inputtext);
            this->Controls->Add(this->viewtext);
            this->MaximizeBox = false;
            this->Name = L"MainWindow";
            this->ShowIcon = false;
            this->Activated += gcnew System::EventHandler(this, &MainWindow::MainWindow_Activated);
            this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MainWindow::MainWindow_FormClosed);
            this->tabControl->ResumeLayout(false);
            this->tabPageFriends->ResumeLayout(false);
            this->tabPageGuild->ResumeLayout(false);
            this->tabPageSettings->ResumeLayout(false);
            this->tabPageSettings->PerformLayout();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion

    private:
        System::Void FriendAddButton_Click(System::Object^ sender, System::EventArgs^  e);

        System::Void MainWindow_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e);

        System::Void MainWindow_Activated(System::Object^  sender, System::EventArgs^  e);
        System::Void inputtext_PreviewKeyDown(System::Object^  sender, System::Windows::Forms::PreviewKeyDownEventArgs^  e);
        System::Void inputtext_KeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e);
        System::Void FriendsListbox_DoubleClick(System::Object^  sender, System::EventArgs^  e);
        System::Void GuildListbox_DoubleClick(System::Object^  sender, System::EventArgs^  e);
        System::Void scrollingCheckbox_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
        System::Void FriendRemoveButton_Click(System::Object^  sender, System::EventArgs^  e);
};

}
