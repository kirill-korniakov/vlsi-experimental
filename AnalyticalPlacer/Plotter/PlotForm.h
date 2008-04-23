#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace Plotter {

	/// <summary>
	/// Summary for PlotForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class PlotForm : public System::Windows::Forms::Form
	{
	public:
		PlotForm(void)
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
		~PlotForm()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::PictureBox^  pbPlotter;
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
      this->pbPlotter = (gcnew System::Windows::Forms::PictureBox());
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbPlotter))->BeginInit();
      this->SuspendLayout();
      // 
      // pbPlotter
      // 
      this->pbPlotter->BackColor = System::Drawing::Color::White;
      this->pbPlotter->Dock = System::Windows::Forms::DockStyle::Fill;
      this->pbPlotter->Location = System::Drawing::Point(0, 0);
      this->pbPlotter->Name = L"pbPlotter";
      this->pbPlotter->Size = System::Drawing::Size(983, 779);
      this->pbPlotter->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
      this->pbPlotter->TabIndex = 0;
      this->pbPlotter->TabStop = false;
      // 
      // PlotForm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(983, 779);
      this->Controls->Add(this->pbPlotter);
      this->Name = L"PlotForm";
      this->Text = L"PlotForm";
      this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &PlotForm::PlotForm_FormClosing);
      (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pbPlotter))->EndInit();
      this->ResumeLayout(false);

    }
#pragma endregion
  private: System::Void PlotForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) 
           {
             this->Visible = false;
             e->Cancel  = true;
           }
  };
}
