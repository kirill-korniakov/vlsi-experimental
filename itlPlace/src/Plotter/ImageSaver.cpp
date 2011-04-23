#include "HPlotter.h"
#include "PlotterData.h"
#include "HDesign.h"
#include "Auxiliary.h"

#include "Base64.h"

static void SavePlotterImage(libconfig::ConfigExt& cfg, const std::string& fileName, const CvArr* image, bool compress, int paletteSize)
{
    const char* compressor = cfg.ValueOf("plotter.PNGCompressor", "pngquant.exe");
    if (compress && (0 == fileName.compare(fileName.length() - 4, 4, ".png")) && Aux::FileExists(compressor))
    {
        cvSaveImage("temp.png", image);
        char cmdline[128];
        sprintf(cmdline, "%s  -force -quiet %d temp.png", compressor, paletteSize);
        system(cmdline);
        if (0 != rename("temp-fs8.png", fileName.c_str()))
        {
            if (0 != rename("temp.png", fileName.c_str()))
            {
                remove("temp.png");
                cvSaveImage(fileName.c_str(), image);
            }
            remove("temp-fs8.png");
        }
        else
            remove("temp.png");
    }
    else
        cvSaveImage(fileName.c_str(), image);
}

void HPlotter::SaveImage(string fileName, string dirName)
{
    if (!IsEnabled())
        return;

    string ImageFileName;
    if (fileName == "")
    {
        ImageFileName = Aux::CreateCoolFileName(dirName, m_hd.Circuit.Name(), "png");
    }
    else
    {
        ImageFileName = Aux::CreateCoolFileName(dirName, fileName, "png");
    }
    //cvSaveImage(ImageFileName.c_str(), m_Data->img);
    SavePlotterImage(m_hd.cfg, ImageFileName, m_Data->img,
        m_hd.cfg.ValueOf("plotter.CompressImages", false),
        m_hd.cfg.ValueOf("plotter.ImagesPaletteSize", 256));
}

void HPlotter::SaveMilestoneImage(string fileSuffix, bool addToHtmlLog)
{
    if (!IsEnabled())
        return;

    if (!m_hd.cfg.ValueOf("plotter.saveMilestoneImages", false))
        return;

    static int milestoneIdx = 0;
    string fileName = Aux::CreateCoolFileName(
        m_hd.cfg.ValueOf("plotter.milestonePixDirectory", "."), 
        m_hd.Circuit.Name() + "_" + Aux::IntToString(milestoneIdx++, 3) + "_" + fileSuffix, 
        "png");

    SavePlotterImage(m_hd.cfg, fileName, m_Data->img,
        m_hd.cfg.ValueOf("plotter.CompressMilestoneImages", true),
        m_hd.cfg.ValueOf("plotter.MilestonePaletteSize", 16));

    if (Logger::Global.HasHTMLStream() && addToHtmlLog)
    {
        bool embeed = m_hd.cfg.ValueOf("plotter.embeedMilestones", false);
        Logger::Global.WriteToHTMLStream(false, "%s", "<div class=\"milestone\"><img alt=\"");
        Logger::Global.WriteToHTMLStream(true, "%s", fileName.c_str());
        if (embeed && Aux::FileExists(fileName))
        {
            int fsize = Aux::GetFileSize(fileName);
            unsigned char* buffer = new unsigned char[fsize];
            try
            {
                FILE* fimg = fopen(fileName.c_str(), "rb");
                if (fimg != 0)
                {
                    fsize = int(fread(buffer, 1, fsize, fimg));
                    fclose(fimg);
                }
                Logger::Global.WriteToHTMLStream(
                    false, 
                    "\" src=\"data:image/jpeg;base64,%s\" /></div>\n", 
                    Base64::Encode(buffer, fsize).c_str());
            }
            catch(...)
            {
                delete[] buffer;
                throw;
            }
            delete[] buffer;
        }
        else
        {
            for (size_t i = 0; i < fileName.length(); ++i)
                if (fileName[i] == '\\')
                    fileName[i] = '/';
            Logger::Global.WriteToHTMLStream(false, "\" src=\"../%s\" /></div>\n", fileName.c_str());
        }
    }
}