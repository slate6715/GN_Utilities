/* 
 * File:   Image.cpp
 * Author: root
 * 
 * Created on October 26, 2012, 7:45 AM
 */

#ifdef _WIN32
#include "stdafx.h"
#endif
#include "Image.h"
#include <limits.h>
#include "miscutils.h"

#ifdef USE_OPENCV

namespace util {
    
Image::Image():
          _x_size(0)
        , _y_size(0)
        , _imgID(UINT_MAX)
        , _docID(UINT_MAX)
        , _location(UINT_MAX)
        , _category(UINT_MAX) 
{
}

Image::~Image(void) {
}

void Image::loadData(DBase &conn, unsigned int imgID) {
    std::unique_ptr<util::Query> stmt = conn.getQueryObj();

    _imgID = imgID;
    *stmt << "SELECT images.Name, images.Path, Location, docID, idImages, documents.category FROM images INNER JOIN documents "
            "ON docID=documents.idDocument WHERE idImages=" << imgID << ";";

    std::unique_ptr<util::UseQueryResults> rs = stmt->execUseQuery();

    if (!rs->fetch_row())
        throw DBException("(ImageData::LoadData) Unable to load the given image data.");

    loadData(rs);
}

void Image::loadData(std::unique_ptr<util::UseQueryResults> &rs) {
    if (!rs)
        throw RuntimeException("(ImageData:LoadData) UseQueryResult was NULL");

    _image.release();
    rs->getString("Name", _name);
    rs->getString("Path", _img_path);
    _location = rs->getInt("Location");
    _docID = rs->getInt("docID");
    _imgID = rs->getInt("idImages");
    if (rs->hasField("Category"))
        _category = rs->getInt("Category");
}

void Image::loadData(std::unique_ptr<util::StoreQueryResults> &rs, unsigned int idx) {
    if (!rs)
        throw util::RuntimeException("(ImageData:LoadData) UseQueryResult was NULL");

    _image.release();
    rs->getString(idx, "Name", _name);
    rs->getString(idx, "Path", _img_path);
    _location = rs->getInt(idx, "Location");
    _docID = rs->getInt(idx, "docID");
    _imgID = rs->getInt(idx, "idImages");
    if (rs->hasField("Category"))
        _category = rs->getInt(idx, "Category");
}

// Load the image from disk using the path already set

bool Image::loadImageP(const char *prefix) {
    std::string fullname = prefix;
    fullname.append("/");
    fullname.append(_img_path);
	return loadImage(fullname.c_str());
}

// Load the image from disk based on the given path

bool Image::loadImageP(const char *prefix, const char *filename) {
    _img_path = filename;
    return loadImage(prefix);
}

bool Image::loadImage(const char *filename) {
	_image = cv::imread(filename);

	if (_image.empty())
		return false;

	_x_size = _image.cols;
	_y_size = _image.rows;

	return true;
}

bool Image::loadImage(std::vector<unsigned char> &img) {
	_image = cv::imdecode(img, CV_LOAD_IMAGE_COLOR);

	if (_image.empty())
		return false;

	_x_size = _image.cols;
	_y_size = _image.rows;

	return true;
}

void Image::saveImage(const char *filename) {
	cv::imwrite(filename, _image);
}

// A method to manually set the image (and path) for this object

void Image::setImage(cv::Mat img, const char *path) {
    if (img.empty())
        throw IOException("(ImageData::SetImage) Image passed in was empty");

    _image = img;
    _img_path = path;

    _x_size = img.cols;
    _y_size = img.rows;
}


/*
bool ImageData::WriteToFile(CStdioFile &file) {
        if (file.m_hFile == CFile::hFileNull)
                return false;

        file.WriteString(_T("ImageData\r\n"));

        CString output;
        output.Format(_T("%s\r\n"), ident);
        file.WriteString(output);
        output.Format(_T("%d\r\n"), x_size);
        file.WriteString(output);
        output.Format(_T("%d\r\n"), y_size);
        file.WriteString(output);
	
        file.WriteString(img_path);
        file.WriteString(_T("\r\n"));

        file.WriteString(_T("SigList\r\n"));

        std::map<CString, Signature *, wltstr>::iterator it = sigs.begin();
        while (iter != sigs.end())
        {
                (iter->second)->WriteToFile(file);
                iter++;
        }

        file.WriteString(_T("EndSigList\r\n"));
        return true;
}

bool ImageData::ReadFromFile(CStdioFile &file, bool expect_hdr) {
        if (file.m_hFile == CFile::hFileNull)
                return false;

        if (expect_hdr)
        {
                CString type;
                file.ReadString(type);
                if (type != "ImageData\r")
                        return false;
        }

        file.ReadString(ident);
        ident.TrimRight();

        CString input;
        file.ReadString(input);
        const TCHAR* numptr = (LPCTSTR) input;
        x_size = wcstol(numptr, NULL, 10);
        file.ReadString(input);
        numptr = (LPCTSTR) input;
        y_size = wcstol(numptr, NULL, 10);

        file.ReadString(img_path);
        img_path.TrimRight();

        file.ReadString(input);
        input.TrimRight();
        if (input != "SigList")
                return false;

        file.ReadString(input);
        while (input != "EndSigList\r")
        {
                if (input == "NDistSig\r")
                {
                        NDistSig *new_sig = new NDistSig();
                        if (!new_sig->ReadFromFile(file, false))
                                return false;
                        AddSignature(new_sig);
                }
                else if (input == "MNDistSig\r")
                {
                        MNDistSig *new_sig = new MNDistSig();
                        if (!new_sig->ReadFromFile(file, false))
                                return false;
                        AddSignature(new_sig);
                }
                else if (input == "SectorMeanSig\r")
                {
                        SectorMeanSig *new_sig = new SectorMeanSig();
                        if (!new_sig->ReadFromFile(file, false))
                                return false;
                        AddSignature(new_sig);
                }
                else
                        return false;

                file.ReadString(input);
        }

        return true;
}


void ImageData::UnloadImage()
{
        image.release();
}

bool ImageData::LoadImage()
{
        CT2CA pszConvertedFilename(img_path);
        cv::Mat loadimage = cv::imread((LPCSTR) pszConvertedFilename);

        x_size = loadimage.cols;
        y_size = loadimage.rows;

        if (loadimage.empty())
                return false;

        image = loadimage;
        return true;
}

bool ImageData::AddSignature(Signature *new_sig)
{
        // Can't add two with the same identifier
        if (GetSignature(new_sig->GetIdent()))
                return false;

        sigs.insert(std::pair<CString, Signature *>(new_sig->GetIdent(), new_sig));
        return true;
}


Signature *ImageData::GetSignature(LPCTSTR ident)
{
        std::map<CString, Signature *, wltstr>::iterator it;

        if ((it = sigs.find(ident)) == sigs.end())
                return NULL;

        return (it->second);
}

int ImageData::RemoveSignature(LPCTSTR ident)
{
        std::map<CString, Signature *, wltstr>::iterator it;

        if ((it = sigs.find(ident)) == sigs.end())
                return 0;

        delete (it->second);
        sigs.erase(it);
        return 1;
}

void ImageData::ResetSigIter()
{
        iter = sigs.begin();
}


Signature *ImageData::GetNext()
{
        if (iter == sigs.end())
                return NULL;

        return ((iter++)->second);
}

int ImageData::GenerateSig(LPCTSTR sig_ident, ImgProcessor &iproc, DlgLink *dlg_link, 
                                                                int replace_dups, int &_isDying)
{
        if (_isDying)
                return 0;

        dlg_link->AddMessage(MSG_ROUTINE1, _T("Processing Image: %s\r\n"), GetIdent());

        // See if the signature already exists
        if (GetSignature(sig_ident))
        {
                if (replace_dups)
                {
                        RemoveSignature(sig_ident);
                        dlg_link->AddMessage(MSG_ROUTINE1, _T("Removing duplicate signature from %s\r\n"),
                                                                                                                                                                        GetIdent());
                }
                else
                {
                        dlg_link->AddMessage(MSG_ROUTINE1, _T("Duplicate signature found, skipping %s\r\n"),
                                                                                                                                                                        GetIdent());
                        return 0;
                }
        }

        if (GetImage().empty())
        {
                if (!LoadImage())
                {
                        dlg_link->AddMessage(MSG_ERROR, _T("Could not load image: %s\r\n"), GetPathName());
                        return 0;
                }
        }

        // go through and tally up our statistics so we can prep to calculate
        Cluster *cptr = dynamic_cast<Cluster*>(&iproc);
        SectMeanProc *sptr = dynamic_cast<SectMeanProc*>(&iproc);
        cv::Mat submat;
        if (cptr != NULL)
        {
                cv::Mat maxvals;
                if (!cptr->GenerateData(submat = GetImage(), maxvals))
                        return 0;

                if (_isDying)
                        return 0;

                cptr->cluster(cv::TermCriteria(), 1, cv::KMEANS_PP_CENTERS, 
                                submat = maxvals(cv::Range::all(), cv::Range(0, maxvals.cols-1)), _isDying, &messages, msgflags);

                KMeans *kmptr = dynamic_cast<KMeans*>(cptr);
                if (kmptr != NULL)
                {
                        NDistSig *nsig_ptr = dynamic_cast<NDistSig*>(kmptr->GenerateSignature(sig_ident));

                        ASSERT(nsig_ptr != NULL);
                        nsig_ptr->SortDist();
                        nsig_ptr->SetMaxvals(maxvals);
		
                        AddSignature(nsig_ptr);
                }
                else {
                        ASSERT(0);
                }
        }
        else if (sptr != NULL)
        {
                cv::Mat tmpimg;
                if (sptr->GenerateMeans(tmpimg = GetImage()).empty())
                {
                        messages.AddMessage(MSG_ERROR, msgflags,
                                        _T("Fatal error generating sector means for image '%s'\r\n"), GetIdent());
                        return 0;
                }

                SectorMeanSig *ssig_ptr = sptr->GenerateSignature(sig_ident);
                ASSERT(ssig_ptr != NULL);
                AddSignature(ssig_ptr);
        }
        else {
                ASSERT(0);
        }

        UnloadImage();
        return 1;
	
}*/

// Get the signatures (or indicated signature) resultset (warning, ResultSet must
// be deleted)
/*
mysqlpp::UseQueryResult ImageData::GetSignaturesRS(DBaseConn &conn, const char *sigID) {
    mysqlpp::Query stmt = conn.getQueryObj();

    stmt << "SELECT * FROM image_sigs WHERE imgID=" << GetImgID();
    if (sigID != NULL)
        stmt << " AND sigID='" << sigID << "'";
    stmt << ";";

    return stmt.use();
}*/
/*
Signature *ImageData::LoadSignature(DBaseConn &conn, const char *sigID, NDistSig &nsig, MNDistSig &mnsig,
        SectorMeanSig &ssig) {
    mysqlpp::UseQueryResult rs = GetSignaturesRS(conn, sigID);
    if (!rs)
        throw DBException("(ImageData::LoadSignature) Signature not found in database!");

    mysqlpp::Row currow = rs.fetch_row();
    if (currow)
        throw CodeException("(ImageData:LoadSignature) No rows found in RS signatures");

    std::string sigtype = currow["type"].c_str();

    if (sigtype.compare("NDistSig") == 0) {
        nsig.ReadFromDB(currow, conn);
        return &nsig;
    } else if (sigtype.compare("MNDistSig") == 0) {
        mnsig.ReadFromDB(currow, conn);
        return &mnsig;
    } else if (sigtype.compare("SectorMeanSig") == 0) {
        ssig.ReadFromDB(currow, conn);
        return &ssig;
    } else
        throw CodeException("(ImageData:LoadSignature) Unrecognized signature type.");

    return NULL;
}
*/
} // namespace util

#endif // def USE_OPENCV
