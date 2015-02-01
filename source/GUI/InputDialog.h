/* This source file is part of koostosh's chb project
see README for copyright notice */

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
	/// Summary for InputDialog
	/// </summary>
	public ref class InputDialog : public System::Windows::Forms::Form
	{
	public:
		InputDialog(String^ name)
		{
			InitializeComponent();
            this->Text = name;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~InputDialog()
		{
			if (components)
			{
				delete components;
			}
		}
    private:
        System::Windows::Forms::TextBox^  textBox1;
        System::Windows::Forms::Button^  button1;

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
            this->textBox1 = (gcnew System::Windows::Forms::TextBox());
            this->button1 = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
            // textBox1
            // 
            this->textBox1->Location = System::Drawing::Point(12, 12);
            this->textBox1->Name = L"textBox1";
            this->textBox1->Size = System::Drawing::Size(190, 20);
            this->textBox1->TabIndex = 0;
            // 
            // button1
            // 
            this->button1->Location = System::Drawing::Point(70, 38);
            this->button1->Name = L"button1";
            this->button1->Size = System::Drawing::Size(75, 23);
            this->button1->TabIndex = 1;
            this->button1->Text = L"OK";
            this->button1->UseVisualStyleBackColor = true;
            this->button1->Click += gcnew System::EventHandler(this, &InputDialog::button1_Click);
            // 
            // InputDialog
            // 
            this->AcceptButton = this->button1;
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(214, 70);
            this->Controls->Add(this->button1);
            this->Controls->Add(this->textBox1);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
            this->MaximizeBox = false;
            this->MinimizeBox = false;
            this->Name = L"InputDialog";
            this->ShowIcon = false;
            this->ShowInTaskbar = false;
            this->Text = L"Input";
            this->TopMost = true;
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
    {
        ((MainWindow^)(this->Owner))->InputDialogReturn(msclr::interop::marshal_as<std::string>(textBox1->Text), this->Text);
        this->Close();
    }

    };
}
