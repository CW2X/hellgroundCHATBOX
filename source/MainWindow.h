#pragma once
#include "base_defs.h"
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
		MainWindow(void)
		{
			InitializeComponent();

			Thread^ t = gcnew Thread(gcnew ThreadStart(this,&MainWindow::BackgroundThread));
            t->Start();
		}

        void print_msg();
        void BackgroundThread();
        String^ readData;
	protected:
		~MainWindow()
		{
			if (components)
			{
				delete components;
			}
		}
    private:
        System::Windows::Forms::TextBox^  viewtext;
        System::Windows::Forms::TextBox^  inputtext;
        System::Windows::Forms::Button^  enterbutton;
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
            this->viewtext = (gcnew System::Windows::Forms::TextBox());
            this->inputtext = (gcnew System::Windows::Forms::TextBox());
            this->enterbutton = (gcnew System::Windows::Forms::Button());
            this->SuspendLayout();
            // 
            // viewtext
            // 
            this->viewtext->BackColor = System::Drawing::Color::White;
            this->viewtext->ForeColor = System::Drawing::Color::Black;
            this->viewtext->Location = System::Drawing::Point(12, 12);
            this->viewtext->Multiline = true;
            this->viewtext->Name = L"viewtext";
            this->viewtext->ReadOnly = true;
            this->viewtext->Size = System::Drawing::Size(628, 242);
            this->viewtext->TabIndex = 0;
            // 
            // inputtext
            // 
            this->inputtext->Location = System::Drawing::Point(12, 260);
            this->inputtext->Name = L"inputtext";
            this->inputtext->Size = System::Drawing::Size(536, 20);
            this->inputtext->TabIndex = 1;
            // 
            // enterbutton
            // 
            this->enterbutton->Location = System::Drawing::Point(554, 260);
            this->enterbutton->Name = L"enterbutton";
            this->enterbutton->Size = System::Drawing::Size(85, 19);
            this->enterbutton->TabIndex = 2;
            this->enterbutton->Text = L"Enter";
            this->enterbutton->UseVisualStyleBackColor = true;
            // 
            // MainWindow
            // 
            this->AcceptButton = this->enterbutton;
            this->ClientSize = System::Drawing::Size(652, 292);
            this->Controls->Add(this->enterbutton);
            this->Controls->Add(this->inputtext);
            this->Controls->Add(this->viewtext);
            this->Name = L"MainWindow";
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
	};

}
