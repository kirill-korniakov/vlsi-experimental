#pragma once
#pragma managed(push,off)

#include "..\include\parser.h"
//#include "..\include\output.h"
//#include "..\include\errors.h"
#include "..\include\global.h"

Circuit    circuit;

#pragma managed(pop)

#include "PlotForm.h"

namespace Plotter {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

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
		MainForm()
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
      simpleSound = gcnew Media::SoundPlayer("c:\\Windows\\Media\\chimes.wav");
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

    int imHeight;
    int imWidth;
    int imHeightShift;
  private: System::Windows::Forms::CheckBox^  chbSlideShow;
  private: System::Windows::Forms::Label^  label1;
  public: 
    int logFilePos;
    void PlotPlacement()
    {
      imHeight = pf->pbPlotter->Height;
      imWidth  = pf->pbPlotter->Width;
      imHeightShift  = 50;
      imHeight -= imHeightShift;             // for text output
      Graphics^ grfx = pf->pbPlotter->CreateGraphics();
      //pf->pbPlotter->Brush->Color = clWhite;
      grfx->DrawRectangle(gcnew Pen(Color::Blue), 1,1,500,500);
      
      ///*PlotForm->Image->Width,
      //PlotForm->Image->Height ) );*/
      //PlotCells( numOfNodes, numOfNodes + numOfTerminals, clBlack, true );
      //if (chbCmp->Checked == false)
      //  PlotCells( 0, numOfNodes, clBlue );
      //else
      //  PlotCells( 0, numOfNodes, clBlue, true, true );
      //PlotBinGrid();
      ////LabelMacros(0, numOfNodes + numOfTerminals);
      ////PlotCells( numOfNodes, numOfNodes + numOfTerminals, clBlack, true );

      //AnsiString String;
      //const int textShift = imWidth / 6;
      //const int textDistance = imWidth / 2;
      //char str[512];
      //char* p;

      //strcpy( str, cbBenchmark->Text.c_str() );
      //p = strrchr( str, '\\' );
      //if (p != NULL)
      //{
      //  String = "Benchmark:  " + AnsiString( ++p );
      //}
      //else
      //{
      //  String = "Benchmark:  " + AnsiString( str );
      //}
      //PlotForm->Image->Canvas->Brush->Color = clWhite;
      //PlotForm->Image->Canvas->TextOutA( 10 + textShift, 0, String );

      //String = "# of nodes:  "     + AnsiString(numOfNodes);
      //PlotForm->Image->Canvas->TextOutA( 10 + textShift, 14, String );

      //strcpy( str, plFileName.c_str() );
      //p = strrchr( str, '\\' );
      //if (p != NULL)
      //{
      //  String = "pl file:  " + AnsiString( ++p );
      //}
      //else
      //{
      //  String = "pl file:  " + plFileName;
      //}
      //PlotForm->Image->Canvas->TextOutA( textDistance + textShift, 0, String );

      //String = "# of terminals:  " + AnsiString(numOfTerminals);
      //PlotForm->Image->Canvas->TextOutA( textDistance + textShift, 14, String );

      //String = "# of rows:  " + AnsiString(numOfRows);
      //PlotForm->Image->Canvas->TextOutA( 10 + textShift, 28, String );

      //String = "# of nets:  "    + AnsiString(numOfNets);
      ////String = "  Number of pins:  "    + AnsiString(numOfPins);
      //PlotForm->Image->Canvas->TextOutA( textDistance + textShift, 28, String );
      delete grfx;
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
    Media::SoundPlayer^ simpleSound;

    //Circuit circuit;
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
      this->chbSlideShow = (gcnew System::Windows::Forms::CheckBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->SuspendLayout();
      // 
      // cbBenchmark
      // 
      this->cbBenchmark->FormattingEnabled = true;
      this->cbBenchmark->Location = System::Drawing::Point(12, 38);
      this->cbBenchmark->Name = L"cbBenchmark";
      this->cbBenchmark->Size = System::Drawing::Size(552, 21);
      this->cbBenchmark->TabIndex = 0;
      this->cbBenchmark->Text = L"C:\\Documents and Settings\\Живодеров Артем\\Рабочий стол\\trunk\\AnalyticalPlacer\\Wor" 
        L"kDir\\ibm01\\ibm01.aux";
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
      // chbSlideShow
      // 
      this->chbSlideShow->AutoSize = true;
      this->chbSlideShow->Location = System::Drawing::Point(13, 134);
      this->chbSlideShow->Name = L"chbSlideShow";
      this->chbSlideShow->Size = System::Drawing::Size(106, 17);
      this->chbSlideShow->TabIndex = 7;
      this->chbSlideShow->Text = L"Create slideshow";
      this->chbSlideShow->UseVisualStyleBackColor = true;
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(283, 202);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(35, 13);
      this->label1->TabIndex = 8;
      this->label1->Text = L"label1";
      // 
      // MainForm
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(815, 706);
      this->Controls->Add(this->label1);
      this->Controls->Add(this->chbSlideShow);
      this->Controls->Add(this->btnBrowsePL);
      this->Controls->Add(this->btnExit);
      this->Controls->Add(this->btnPlot);
      this->Controls->Add(this->cbPL);
      this->Controls->Add(this->btnBrowseBench);
      this->Controls->Add(this->cbBenchmark);
      this->Name = L"MainForm";
      this->Text = L"Plotter";
      this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainForm::MainForm_Paint);
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
  private: 
    System::Void btnPlot_Click(System::Object^  sender, System::EventArgs^  e) 
           {
             Circuit circuit;
             //InitializeCircuit(circuit);
             HINSTANCE h;

             // Объявление указателя на функцию, вызываемой из DLL
             // Обратите внимание – имена объявляемой функции и
             // функции, вызываемой из DLL, могут и не совпадать,
             // т.к. за выбор вызываемой функции отвечает
             // GetProcAddress
             MULTIPLACER_ERROR (*DllFunc) (Circuit& circuit);
             int (*ParseAux) (const char* fileName, Circuit& circuit);

             // Загружаем MyFirstDLL
             h=LoadLibrary(L"itlParser.dll");

             // Контроль ошибок – если загрузка прошла успешно,
             // функция вернет что-то отличное от нуля
             if (!h)
             {
               printf("Ошибка - не могу найти itlParser.dll\n");
               return;
             }

             // Вызовом GetProcAddress получаем адрес функции Demo
             // и присваиваем его указателю DllFunc с явным 
             // приведением типов. Это необходимо т.к.
             // GetProcAddress возвращает бестиповой far-указатель
             DllFunc=(MULTIPLACER_ERROR (*) (Circuit& circuit))
               GetProcAddress(h,"InitializeCircuit");
             ParseAux = (int (*) (const char* fileName, Circuit& circuit))
               GetProcAddress(h,"ParseAux");

             // Контроль ошибок – если вызов функции GetProcAddress
             // завершился успешно, она вернет ненулевой указатель
             if (!ParseAux)
             {
               printf("Ошибка! В MyFirstDLL "
                 "отсутствует ф-ция   Demo\n");
               return;
             }
             /*String^ tmpStr = cbBenchmark->Text;
             tmpStr->Replace(L"\\", L"\\\\");*/

             char* auxFileNameChar = (char*) System::Runtime::InteropServices::Marshal::
               StringToHGlobalAnsi(cbBenchmark->Text).ToPointer();
             /*char* tmp = new char[strlen(auxFileNameChar) + 3];
             char* pVal = tmp + 1;
             strcpy(pVal, auxFileNameChar);
             tmp[0] = '\"';
             tmp[strlen(auxFileNameChar) + 1] = '\"';
             tmp[strlen(auxFileNameChar) + 2] = '\0';*/
             
             //strcpy(tmp, "ibm01\\ibm01.aux");

             //ParseAux("ibm01\\ibm01.aux", circuit);
             //FILE * auxFile   = fopen(auxFileNameChar, "r");
             //fgets(tmp, 20, auxFile);

             int error = ParseAux(auxFileNameChar, circuit);
             ParseAux("D:\\Temp\\dma\\dma.aux", circuit);
             //ParseAux("C:\\Documents and Settings\\Живодеров Артем\\Рабочий стол\\trunk\\AnalyticalPlacer\\WorkDir\\ibm01\\ibm01.aux", circuit);
             label1->Text = (String^)(circuit.placement[0].xCoord).ToString();
             //DllFunc(circuit);

             // Выгрузка динамической библиотеки из памяти
             FreeLibrary(h);

             pf->Show();
             PlotPlacement();
             
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
  System::Void button2_Click(System::Object^  sender, System::EventArgs^  e)
         {
           String^ tmpStr;
           Circuit circuit;
           //InitializeCircuit(circuit);
           if (openPLDialog->ShowDialog() == ::DialogResult::OK)
           {
             char* plFileNameChar = (char*) System::Runtime::InteropServices::Marshal::
                                            StringToHGlobalAnsi(plFileName).ToPointer(); 
             cbPL->Text = openPLDialog->FileName;
             for (int i = 0; i < openPLDialog->FileNames->Length; i++)
             {
               plFileName = openPLDialog->FileNames[i];
               //char tmpStr[512];
               //tmpStr = PtrToStringChars(plFileName);               
               if (i == 0) btnPlot->PerformClick();
               if (placement)
               {
                 if (i != 0)
                   //ReparsePl(plFileNameChar, table, numOfNodes + numOfTerminals, placement);
                 openPLDialog->Tag = 1;
                 /*if (chbCmp->Checked)
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
                 }*/
                 if (i != 0 && chbSlideShow->Checked == true)
                   PlotPlacement();
                 if (chbSlideShow->Checked == true)
                 {
                   tmpStr = openPLDialog->FileNames[i];
                   tmpStr->Remove(tmpStr->Length - 2, 2);
                   tmpStr += "jpeg";
                   /*jpegImage->Assign(PlotForm->Image->Picture->Bitmap);
                   jpegImage->SaveToFile(tmpStr);*/
                 }
                 btnPlot->Tag = 1;
               }
             }
             //System::Runtime::InteropServices::Marshal::FreeHGlobal(plFileNameChar);
           }
         }
private: System::Void MainForm_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e)
         {
           //simpleSound->Play();
         }
};
}