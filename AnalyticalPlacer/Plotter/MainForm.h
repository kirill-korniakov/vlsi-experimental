#pragma once
#include "..\include\parser.h"
#include "PlotForm.h"
//#include "..\include\output.h"
//#include "..\include\errors.h"
//#include "..\include\global.h"

namespace Plotter {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

  extern str* table;
  extern strExtend* newTable;

	/// <summary>
	/// Summary for MainForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
      pf = gcnew PlotForm();
      pf->Visible = false;
      pf->StartPosition = FormStartPosition::CenterScreen;
      pf->ShowInTaskbar = false;
      pf->WindowState = FormWindowState::Maximized;
      logFilePos = 0;
      oldFileLen = 0;
      newFileLen = 0;
      rows         = NULL;
      nets         = NULL;
      nodes        = NULL;
      terminals    = NULL;
      weights      = NULL;
      placement    = NULL;
      tableOfNames = NULL;
      arrOfBins    = NULL;
      //table        = NULL;
      //cellColors   = NULL;
      //Application:: = "itlDragon placer";
      plFileName = "";
      InitializeComponent();
      ((Bitmap^)(btnBrowseBench->Image))->MakeTransparent(/*Color::FromArgb(255,255,0,255)*/);
      ((Bitmap^)(btnBrowsePL->Image))->MakeTransparent(/*Color::FromArgb(255,255,0,255)*/);
		}
  private: System::Windows::Forms::Button^  btnExit;
  private: System::Windows::Forms::Button^  btnBrowsePL;

  private: System::Windows::Forms::OpenFileDialog^  openAuxDialog;
  private: System::Windows::Forms::OpenFileDialog^  openPLDialog;
  public: 
    PlotForm^ pf;
    String^ dirName;
    String^ plFileName;
    //TJPEGImage *jpegImage;
    Color^ cellColors;
    int numOfRows;
    int numOfNets;
    int numOfPins;
    int numOfNodes;
    int numOfTerminals;
    int nBinRows;
    int nBinCols;
    int rowLen;
    int circuitHeight;
    int oldFileLen;
    int newFileLen;
    int logFilePos;
    void PlotPlacement()
    {

    }
    void PlotCells( int first, int last, Color color, bool isSolid, bool isGradient )
    {

    }
    void PlotBinGrid(Color color)
    {

    }
    void LabelMacros(int first, int last)
    {

    }
    //int reparse_pl(const char* fileName, str* table, int numOfNT, Place** placement);
    Row* rows         ;
    DataStructures::Net* nets         ;
    Node* nodes       ;
    Node* terminals   ;
    Weight* weights   ;
    Place* placement  ;
    str* tableOfNames ;
    pBin **arrOfBins  ;
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::ComboBox^  cbBenchmark;
  private: System::Windows::Forms::Button^  btnBrowseBench;
  protected: 

  protected: 

  private: System::Windows::Forms::ComboBox^  cbPL;



  private: System::Windows::Forms::Button^  btnPlot;

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
      System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
      this->cbBenchmark = (gcnew System::Windows::Forms::ComboBox());
      this->btnBrowseBench = (gcnew System::Windows::Forms::Button());
      this->cbPL = (gcnew System::Windows::Forms::ComboBox());
      this->btnPlot = (gcnew System::Windows::Forms::Button());
      this->btnExit = (gcnew System::Windows::Forms::Button());
      this->btnBrowsePL = (gcnew System::Windows::Forms::Button());
      this->openAuxDialog = (gcnew System::Windows::Forms::OpenFileDialog());
      this->openPLDialog = (gcnew System::Windows::Forms::OpenFileDialog());
      this->SuspendLayout();
      // 
      // cbBenchmark
      // 
      this->cbBenchmark->FormattingEnabled = true;
      this->cbBenchmark->Location = System::Drawing::Point(12, 38);
      this->cbBenchmark->Name = L"cbBenchmark";
      this->cbBenchmark->Size = System::Drawing::Size(552, 21);
      this->cbBenchmark->TabIndex = 0;
      // 
      // btnBrowseBench
      // 
      this->btnBrowseBench->Cursor = System::Windows::Forms::Cursors::Hand;
      this->btnBrowseBench->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
      this->btnBrowseBench->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"btnBrowseBench.Image")));
      this->btnBrowseBench->Location = System::Drawing::Point(582, 36);
      this->btnBrowseBench->Name = L"btnBrowseBench";
      this->btnBrowseBench->Size = System::Drawing::Size(25, 23);
      this->btnBrowseBench->TabIndex = 1;
      this->btnBrowseBench->TabStop = false;
      this->btnBrowseBench->UseVisualStyleBackColor = true;
      this->btnBrowseBench->Click += gcnew System::EventHandler(this, &MainForm::button1_Click);
      // 
      // cbPL
      // 
      this->cbPL->FormattingEnabled = true;
      this->cbPL->Location = System::Drawing::Point(12, 89);
      this->cbPL->Name = L"cbPL";
      this->cbPL->Size = System::Drawing::Size(552, 21);
      this->cbPL->TabIndex = 2;
      // 
      // btnPlot
      // 
      this->btnPlot->Location = System::Drawing::Point(627, 87);
      this->btnPlot->Name = L"btnPlot";
      this->btnPlot->Size = System::Drawing::Size(75, 23);
      this->btnPlot->TabIndex = 4;
      this->btnPlot->Text = L"Plot";
      this->btnPlot->UseVisualStyleBackColor = true;
      this->btnPlot->Click += gcnew System::EventHandler(this, &MainForm::btnPlot_Click);
      // 
      // btnExit
      // 
      this->btnExit->Location = System::Drawing::Point(721, 87);
      this->btnExit->Name = L"btnExit";
      this->btnExit->Size = System::Drawing::Size(75, 23);
      this->btnExit->TabIndex = 5;
      this->btnExit->Text = L"Exit";
      this->btnExit->UseVisualStyleBackColor = true;
      this->btnExit->Click += gcnew System::EventHandler(this, &MainForm::btnExit_Click);
      // 
      // btnBrowsePL
      // 
      this->btnBrowsePL->Cursor = System::Windows::Forms::Cursors::Hand;
      this->btnBrowsePL->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
      this->btnBrowsePL->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"btnBrowsePL.Image")));
      this->btnBrowsePL->Location = System::Drawing::Point(582, 87);
      this->btnBrowsePL->Name = L"btnBrowsePL";
      this->btnBrowsePL->Size = System::Drawing::Size(25, 23);
      this->btnBrowsePL->TabIndex = 6;
      this->btnBrowsePL->TabStop = false;
      this->btnBrowsePL->UseVisualStyleBackColor = true;
      this->btnBrowsePL->Click += gcnew System::EventHandler(this, &MainForm::button2_Click);
      // 
      // openAuxDialog
      // 
      this->openAuxDialog->DefaultExt = L"aux";
      this->openAuxDialog->Filter = L"Auxiliary files (*.aux)|*.aux";
      // 
      // openPLDialog
      // 
      this->openPLDialog->FileName = L"temp.pl";
      this->openPLDialog->Filter = L"Placement files(*.pl)|*.pl";
      // 
      // MainForm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(815, 706);
      this->Controls->Add(this->btnBrowsePL);
      this->Controls->Add(this->btnExit);
      this->Controls->Add(this->btnPlot);
      this->Controls->Add(this->cbPL);
      this->Controls->Add(this->btnBrowseBench);
      this->Controls->Add(this->cbBenchmark);
      this->Name = L"MainForm";
      this->Text = L"Plotter";
      this->ResumeLayout(false);

    }
#pragma endregion
  private: System::Void btnPlot_Click(System::Object^  sender, System::EventArgs^  e) 
           {
             pf->Show();
             //pf->Size = pf->MaximumSize;
           }
private: System::Void btnExit_Click(System::Object^  sender, System::EventArgs^  e)
         {
           this->Close();
         }
private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
         {
           if (openAuxDialog->ShowDialog() == ::DialogResult::OK)
           {
             cbBenchmark->Text = openAuxDialog->FileName;
           }
         }

private: 

#pragma managed(push, off)
  System::Void button2_Click(System::Object^  sender, System::EventArgs^  e)
         {
           if (openPLDialog->ShowDialog() == ::DialogResult::OK)
           {
             char* plFileNameChar = (char*) System::Runtime::InteropServices::Marshal::
                                            StringToHGlobalAnsi(plFileName).ToPointer(); 
             cbPL->Text = openPLDialog->FileName;
             for (int i = 0; i < openPLDialog->FileNames->Length; i++)
             {
               plFileName = openPLDialog->FileNames[i];
               char tmpStr[512];
               //tmpStr = PtrToStringChars(plFileName);               
               if (i == 0) btnPlot->PerformClick();
               if (placement)
               {
                 if (i != 0)
                   ReparsePl(plFileNameChar, table, numOfNodes + numOfTerminals, placement, nBinRows, nBinCols);
                 openPLDialog->Tag = 1;
                 if (chbCmp->Checked)
                 {
                   int tmpX;
                   int tmpY;
                   for (int i = 0; i < numOfNodes; i++)
                   {
                     tmpX = (int)(placement[i].xCoord * 255 / rowLen);
                     cellColors[i] = tmpX;
                     tmpY = (int)(placement[i].yCoord * 255 / circuitHeight);
                     tmpY = tmpY << 8;
                     cellColors[i] += tmpY;
                   }
                 }
                 if (i != 0 && chbSlideShow->Checked == true)
                   PlotPlacement();
                 if (/*OpenPLDialog->Files->Count > 1 && */chbSlideShow->Checked == true)
                 {
                   tmpStr = OpenPLDialog->Files->Strings[i];
                   tmpStr.SetLength(tmpStr.Length() - 2);
                   tmpStr += "jpeg";
                   jpegImage->Assign(PlotForm->Image->Picture->Bitmap);
                   jpegImage->SaveToFile(tmpStr);
                 }
                 btnPlot->Tag = 1;
               }
             }
             System::Runtime::InteropServices::Marshal::FreeHGlobal(plFileNameChar);
           }
         }
  #pragma managed(pop) 
};
}

