#pragma once
#include "MainWindow.h"

namespace chb {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for LoginForm
	/// </summary>
	public ref class LoginForm : public System::Windows::Forms::Form
	{
	public:
		LoginForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~LoginForm()
		{
			if (components)
			{
				delete components;
			}
		}
        public: System::Windows::Forms::Button^  CancelButton;
        private: System::Windows::Forms::TextBox^  LoginBox;
        public:
        private: System::Windows::Forms::TextBox^  PasswordBox;
        private: System::Windows::Forms::Label^  label1;
        private: System::Windows::Forms::Label^  label2;
        public: System::Windows::Forms::Button^  LoginButton;
        private:

        private:
        protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->CancelButton = (gcnew System::Windows::Forms::Button());
            this->LoginBox = (gcnew System::Windows::Forms::TextBox());
            this->PasswordBox = (gcnew System::Windows::Forms::TextBox());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->LoginButton = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
            // CancelButton
            // 
            this->CancelButton->Location = System::Drawing::Point(116, 65);
            this->CancelButton->Name = L"CancelButton";
            this->CancelButton->Size = System::Drawing::Size(60, 23);
            this->CancelButton->TabIndex = 0;
            this->CancelButton->Text = L"Cancel";
            this->CancelButton->UseVisualStyleBackColor = true;
            this->CancelButton->Click += gcnew System::EventHandler(this, &LoginForm::CancelButton_Click);
            // 
            // LoginBox
            // 
            this->LoginBox->Location = System::Drawing::Point(51, 13);
            this->LoginBox->Name = L"LoginBox";
            this->LoginBox->Size = System::Drawing::Size(125, 20);
            this->LoginBox->TabIndex = 1;
            // 
            // PasswordBox
            // 
            this->PasswordBox->Location = System::Drawing::Point(51, 39);
            this->PasswordBox->Name = L"PasswordBox";
            this->PasswordBox->PasswordChar = '*';
            this->PasswordBox->Size = System::Drawing::Size(125, 20);
            this->PasswordBox->TabIndex = 2;
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(12, 16);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(36, 13);
            this->label1->TabIndex = 3;
            this->label1->Text = L"Login:";
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(12, 42);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(39, 13);
            this->label2->TabIndex = 4;
            this->label2->Text = L"Has�o:";
            // 
            // LoginButton
            // 
            this->LoginButton->Location = System::Drawing::Point(51, 65);
            this->LoginButton->Name = L"LoginButton";
            this->LoginButton->Size = System::Drawing::Size(59, 23);
            this->LoginButton->TabIndex = 5;
            this->LoginButton->Text = L"Login";
            this->LoginButton->UseVisualStyleBackColor = true;
            this->LoginButton->Click += gcnew System::EventHandler(this, &LoginForm::LoginButton_Click);
            // 
            // LoginForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(186, 101);
            this->ControlBox = false;
            this->Controls->Add(this->LoginButton);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->PasswordBox);
            this->Controls->Add(this->LoginBox);
            this->Controls->Add(this->CancelButton);
            this->KeyPreview = true;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"LoginForm";
            this->Text = L"Login";
            this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &LoginForm::LoginForm_KeyDown);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
        private:
        System::Void CancelButton_Click(System::Object^  sender, System::EventArgs^  e)
        {
            Close();
        }

    private:
        System::Void LoginButton_Click(System::Object^  sender, System::EventArgs^  e)
        {
            if (String::IsNullOrEmpty(LoginBox->Text) || String::IsNullOrEmpty(PasswordBox->Text))
                return;

            Form^ form = gcnew MainWindow(LoginBox->Text, PasswordBox->Text);
            form->TopMost = false;
            form->StartPosition = FormStartPosition::CenterScreen;
            form->Show(this);

            Hide();
        }
    private:
        System::Void LoginForm_KeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
        {
            if (e->KeyCode == Keys::Enter)
                LoginButton_Click(sender, e);
        }
};
}
