#pragma once

#pragma unmanaged
#include "essworld.h"

#pragma managed

namespace ess_01 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Collections::Generic;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
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
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Button^  button1;
	private: System::ComponentModel::IContainer^  components;
	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->button1 = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(212, 32);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(434, 301);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(582, 403);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"start";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(7, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(796, 522);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->pictureBox1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
		essWorld * pWorld;
		population * pop;
		Graphics ^_g;
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
				 pop = new population();
				pWorld = new essWorld();
				pop->Init(pWorld);
				//this->timer1->Start();

				_g = this->pictureBox1->CreateGraphics();
				_BlackList = gcnew System::Collections::Generic::Queue<int>();
				_BlueList = gcnew System::Collections::Generic::Queue<int>();
			 }
			 //cli::array<Point^,
			 
			 
			 System::Collections::Generic::Queue<int> ^ _BlackList ;//= gcnew List<Point>();
			 System::Collections::Generic::Queue<int> ^ _BlueList ;//= gcnew List<Point>();

				void AddValue(int black,int blue)
				{
					_BlackList->Enqueue(black);
					if( _BlackList->Count == 50)
						_BlackList->Dequeue();
					_BlueList->Enqueue(blue);
					if( _BlueList->Count == 50)
						_BlueList->Dequeue();

				}

	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {

				pop->evaluate();
				
				AddValue(pWorld->_blackCount,pWorld->_blueCount);
				
				pWorld->_blackCount = 0;
				pWorld->_blueCount = 0;

				_g->Clear(Color::White);

				cli::array<int> ^ black =	_BlackList->ToArray();
				for(int i = 1 ; i < black->Length;i++)
				{
					_g->DrawLine(gcnew Pen(Color::Red),(i-1)*3,300-black[i-1]*3,i*3,300-black[i]*3);
				}

				cli::array<int> ^ blue =	_BlueList->ToArray();
				for(int i = 1 ; i < blue->Length;i++)
				{
					_g->DrawLine(gcnew Pen(Color::Black),(i-1)*3,300-blue[i-1]*3,i*3,300-blue[i]*3);
				}
				
			 }
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 
				 if( this->button1->Text == "start")
				 {
					this->timer1->Start();
					this->button1->Text = "stop";
				 }else
				 {
					this->button1->Text = "start";
					this->timer1->Stop();
				 }
				 

			 }
};
}

