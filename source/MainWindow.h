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
            this->viewtext->Size = System::Drawing::Size(628, 242);
            this->viewtext->TabIndex = 0;
            // 
            // inputtext
            // 
            this->inputtext->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->inputtext->Location = System::Drawing::Point(12, 260);
            this->inputtext->Name = L"inputtext";
            this->inputtext->Size = System::Drawing::Size(628, 20);
            this->inputtext->TabIndex = 1;
            this->inputtext->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MainWindow::inputtext_KeyPress);
            this->inputtext->PreviewKeyDown += gcnew System::Windows::Forms::PreviewKeyDownEventHandler(this, &MainWindow::inputtext_PreviewKeyDown);
            // 
            // MainWindow
            // 
            this->ClientSize = System::Drawing::Size(652, 292);
            this->Controls->Add(this->inputtext);
            this->Controls->Add(this->viewtext);
            this->MaximizeBox = false;
            this->Name = L"MainWindow";
            this->ShowIcon = false;
            this->Activated += gcnew System::EventHandler(this, &MainWindow::MainWindow_Activated);
            this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MainWindow::MainWindow_FormClosed);
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
