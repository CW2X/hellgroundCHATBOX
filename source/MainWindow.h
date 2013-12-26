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

    typedef uint8 (*mainDllUpdateType)(std::string*);
    typedef void (*mainDllInputType)(std::string);

	public ref class MainWindow : public System::Windows::Forms::Form
	{
	public:
		MainWindow()
		{
            ExitingProgram = false;
			InitializeComponent();

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
        System::Windows::Forms::Button^  enterbutton;
		System::ComponentModel::Container ^components;
        Thread^ backThread;
        bool ExitingProgram;
        mainDllUpdateType mainDllUpdateFunction;
        mainDllInputType mainDllInputFunction;

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
            this->viewtext->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
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
            this->enterbutton->Click += gcnew System::EventHandler(this, &MainWindow::enterbutton_Click);
            // 
            // MainWindow
            // 
            this->AcceptButton = this->enterbutton;
            this->ClientSize = System::Drawing::Size(652, 292);
            this->Controls->Add(this->enterbutton);
            this->Controls->Add(this->inputtext);
            this->Controls->Add(this->viewtext);
            this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
            this->MaximizeBox = false;
            this->Name = L"MainWindow";
            this->ShowIcon = false;
            this->Text = (L"Chatbox "+ VERSION +" by HGdev team");
            this->Activated += gcnew System::EventHandler(this, &MainWindow::MainWindow_Activated);
            this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MainWindow::MainWindow_FormClosed);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private:
        System::Void enterbutton_Click(System::Object^  sender, System::EventArgs^  e)
        {
            if (mainDllInputFunction != NULL)
            mainDllInputFunction(msclr::interop::marshal_as<std::string>(inputtext->Text));
            inputtext->Text = gcnew System::String("");
        }

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

        void CreateLoginForm();
};

}
