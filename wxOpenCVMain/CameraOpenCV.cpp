#include "ImageInfo.h"
#include "CameraOpenCV.h"
#include "EvenementCamera.h"
//#include <fstream>
#include <wx/time.h> 

using namespace std;
using namespace cv;

char CameraOpenCV::indIdVideo[NBCAMERA]={0,0,0,0,0,0,0,0,0,0};


//#define _FLUXHTTP__

EvtPointSuivis::EvtPointSuivis(wxEventType commandType , int id):wxCommandEvent(m_sortie,id)
    {
        SetId(id);
        SetEventType(commandType);
        m_Expediteur = 0;
        m_sortie = false;
        m_travailEchoue = false;
		indEvt=0;
    }



CameraOpenCV::CameraOpenCV(wxString nomFlux):CameraVirtuelle()
{
//nomFlux="0";
wxString		repTravail=wxGetCwd();
strcpy(nomCamera,"OpenCV");
wxString nomOrdinateur(wxGetHostName());
tailleCapteur[0]=wxSize(160,120);tailleCapteur[1]=wxSize(176,144);tailleCapteur[2]=wxSize(320,176);
tailleCapteur[3]=wxSize(320,240);tailleCapteur[4]=wxSize(352,288);tailleCapteur[5]=wxSize(432,240);
tailleCapteur[6]=wxSize(544,288);tailleCapteur[7]=wxSize(640,360);tailleCapteur[8]=wxSize(640,480);
tailleCapteur[9]=wxSize(752,416);tailleCapteur[10]=wxSize(800,448);tailleCapteur[11]=wxSize(800,600);
tailleCapteur[12]=wxSize(864,480);tailleCapteur[13]=wxSize(960,544);tailleCapteur[14]=wxSize(960,720);
tailleCapteur[15]=wxSize(1024,576);tailleCapteur[16]=wxSize(1184,656);tailleCapteur[17]=wxSize(1280,720);
tailleCapteur[16]=wxSize(1280,960);tailleCapteur[17]=wxSize(-1,-1);tailleCapteur[18]=wxSize(-1,-1);
tailleCapteur[19]=wxSize(-1,-1);
/*for (int i=15;i<NB_TAILLE_VIDEO;i++)
	tailleAutorisee[false];*/
modeMoyenne=false;
indFiltreMoyenne=1;
tpsInactif =30;
fluxOuvert=false;
aaButter[0]=-0.9996859;
aaButter[1]=-0.9993719;
aaButter[2]=-0.9968633;
aaButter[3]=-0.9937365;
aaButter[4]=-0.9690674;
aaButter[5]=-0.9390625;
aaButter[6]=-0.7265425;
aaButter[7]=-0.5095254;
aaButter[8]=-0.3249;
aaButter[9]=-0.1584;
aaButter[10]=-0.0;
bbButter[0]=0.0001571;
bbButter[1]=0.0003141;
bbButter[2]=0.0015683;
bbButter[3]=0.0031318;
bbButter[4]=0.0154663;
bbButter[5]=0.0304687;
bbButter[6]=0.1367287;
bbButter[7]=0.2452373;
bbButter[8]=0.3375;
bbButter[9]=0.4208;
bbButter[10]=0.5;

parent=NULL;
captureVideo=NULL;
imAcq=NULL;
long indFlux;

// Appel de VideoCapture pour r�cup�rer la taille de la vid�o

if (nomFlux!=wxEmptyString)
	{
	if (nomFlux.ToLong(&indFlux))
		{
		captureVideo = new cv::VideoCapture(indFlux); 
		indId=indFlux;
		}
	else
		{

		captureVideo = new cv::VideoCapture(nomFlux.ToStdString()); 
		indId=-1;
		}
	}
else
	{
	for (indId=0;indId<NBCAMERA ;indId++)
		if (indIdVideo[indId]==0)
			{
			captureVideo = new cv::VideoCapture(indId); 
			if(captureVideo && captureVideo->isOpened())
				break;
			if(captureVideo)
				{
				delete captureVideo;
				captureVideo=NULL;
				}
			}
	if (indId==NBCAMERA)
		return;
	}

if(captureVideo->isOpened())  // check if we succeeded
	{
	cv::Mat frame;
	if( indId==-1)
		fluxOuvert=true;
/*	for (int i=15;i<NB_TAILLE_VIDEO;i++)
		{
		status=captureVideo->set(cv::CAP_PROP_FRAME_WIDTH,tailleCapteur[i].GetX());
		tailleAutorisee[i]=captureVideo->set(cv::CAP_PROP_FRAME_HEIGHT,tailleCapteur[i].GetY());
		}*/
	(*captureVideo) >> frame; // get a new frame from camera
	nbColonnePhys = frame.cols;
	nbLignePhys = frame.rows;
	if (frame.cols*frame.rows==0)
		indId=NBCAMERA;
	switch(frame.type()){
	case CV_8UC3:

		nbCanauxAD = frame.type();
		break;
	default:
		nbCanauxAD = frame.type();
		}
	}
else
	{
	indId=NBCAMERA;
 	return;
	}
if (indId>=0 && indId<NBCAMERA)
	indIdVideo[indId]=1;

chaineErreur="";
char	tmp[1024];
strcpy(tmp,repTravail.mb_str( wxConvUTF8));
testDriver=1;
if (!captureVideo)
	{
	AjouteMsgErreur("Initialization errors:\n");
    AjouteMsgErreur("Initialize Error\n");
    }
//PrepareAcquisition();
indLignePremierpixel=1;
nbLigneInterPiste=1;
colonneDebut=1;
ligneDebut=1;
colonneFin=nbColonnePhys;
ligneFin=nbLignePhys;
//tempsExposition = captureVideo->getMinMax(CV_CAP_PROP_EXPOSURE,&expoMin,&expoMax);
nbScanParAccumulation=1;
periodeAccumulation=1;	// en seconde
modeDeclenchement=0;			// Mode de d�clenchement de l'acquisition p45 SDK Andor

erreurFct=captureVideo->set(CAP_PROP_EXPOSURE,tempsExposition);
if(!erreurFct)
	AjouteMsgErreur("SetExposureTime Error\n");
//captureVideo->getMinMax(CV_CAP_PROP_GAIN,&gainMin,&gainMax);
}

CameraOpenCV::~CameraOpenCV(void)
{
if (parent==NULL)
	{
	if (Connectee() && indId>=0 && indId<NBCAMERA)
		indIdVideo[indId]=0;
	if (captureVideo)
		delete captureVideo;
	}
else
	{
	wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);

	if (Connectee()&& indId>=0 && indId<NBCAMERA)
		indIdVideo[indId]=0;
	if (captureVideo)
		delete captureVideo;
	}
}

void CameraOpenCV::DefTempsExposition(double x)
{
if (!parent)
	return;
wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);
captureVideo->set(CAP_PROP_EXPOSURE,x);
}

void CameraOpenCV::DefModeGain(int i)
{
}

void CameraOpenCV::DefEMCCDGain(int v)
{
}

void CameraOpenCV::LireIntervalGain(int *x,int *y)
{
}

void CameraOpenCV::DefNombreAccumulation(int x)
{
}

void CameraOpenCV::DefFrequenceAccumulation(double x)
{
}

void CameraOpenCV::DefNombreCinetique(int x)
{
}

void CameraOpenCV::DefFrequenceCinetique(double x)
{
}

void CameraOpenCV::DefTailleImage(int article)
{
}

void CameraOpenCV::DefBinning(int article)
{
}

void CameraOpenCV::DefModeAcquisition(int article)
{
}
void CameraOpenCV::DefModeDeclenchement(int article)
{
chaineErreur="";
}
void CameraOpenCV::DefZoneLecture(int article)
{
chaineErreur="";
}


int	CameraOpenCV::Acquisition(void) /*<! Acquisition d'une image */
{
if (typeAcq==CV_32FC3)
	{
	Mat frame;
	Mat frameFlt;
	while (!captureVideo->retrieve(frame));
	wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);
	frame.convertTo(frameFlt,CV_32FC3);
	(*((Mat *)imAcq)) =frameFlt; // get a new frame from camera
	}
else if (typeAcq==CV_8UC3)
	{
	Mat frame;
	while (!captureVideo->retrieve(frame));
	wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);

	(*((Mat *)imAcq)) =frame; // get a new frame from camera
	}

return 1;
}

char CameraOpenCV::ImagePrete()
{
return 0;
}

char CameraOpenCV::Image(unsigned short *data,unsigned long &taille)
{

chaineErreur="";
if (TestDriver()==0)
	{
/*
	ifstream g("imagepardefaut.txt");
	char	nomFichier[1024];
	static ImageInfoCV *imTest= NULL;
	if (!imTest)
		if (g.is_open())
			{
			g.getline(nomFichier,1024);
			imTest = new ImageInfoCV(nomFichier);
			}
		else
			imTest = new ImageInfoCV("C:\\Images\\seec images\\20091102_Treated\\EPC_P89_START_j7h0m29_END_j7h16m17_T45\\EgPC_P89_Day9A45_09.tif");
	unsigned short *dataTest=(unsigned short *)imTest->data;
	for (int i=0;i<(ligneFin-ligneDebut+1);i++)
		for (int j=0;j<colonneFin-colonneDebut+1;j++,data++,dataTest++)
			{
			*data =*dataTest;
			}
	return 1;
*/
	}
taille=0;return 1;
}


char CameraOpenCV::TailleImage(unsigned long &taille)
{
return 1;
};

void CameraOpenCV::DefCoinGauche(int x)
{
return;
}

void CameraOpenCV::DefCoinDroit(int x)
{
return;
}

void CameraOpenCV::DefCoinHaut(int x)
{
return;
}

void CameraOpenCV::DefCoinBas(int x)
{
return;
}

char CameraOpenCV::EMGain8Bit()
{
	return 0;
}

char CameraOpenCV::EMGain12Bit()
{
	return 0;
}

char CameraOpenCV::EMGainLinear12Bit()
{
	return 0;
}

char CameraOpenCV::EMGainReal12Bit()
{
	return 0;
}

	
char CameraOpenCV::GetFunctionEMCCDGain()
{
	return 0;
}

wxThread::ExitCode CameraOpenCV::Acquisition8BitsRGB()
{
int indEvt=0;
if (!captureVideo)
	captureVideo= new VideoCapture(indId); 

if (captureVideo->isOpened())
	{
	Mat frame1;
	Mat frame2;
	Mat frame;
	std::vector<cv::Point2f> repereIni,repere;
	while (!captureVideo->retrieve(frame2));
	while (!captureVideo->retrieve(frame1));
	static bool opActif=false;
	ImageInfoCV *imIni=new ImageInfoCV(frame.rows,frame.cols,frame.flags);
	ImageInfoCV *imPre=NULL;
    std::map<ImageInfoCV*,bool> effaceImage;
	for(;true;)
		{
		if (captureVideo->grab()) // get a new frame from camera
			{
			captureVideo->retrieve(frame);
			
			if (parent)
				{
				if (seqOp.size()!=0)
					{
					opActif =true;
					frame.copyTo(*imIni);
					ImageInfoCV ***im=new ImageInfoCV**[seqOp.size()];
					if (!parent)
						break;
					int indOp=0;
					{
					wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->paramCam);

					if (seqActualisee)
						{
						seqActualisee=false;
						delete imPre;
						imPre=false;
						}
					for (std::vector <ParametreOperation > ::iterator it=seqOp.begin();it!=seqOp.end();it++)
						{
						ParametreOperation pOCV=*it;
						pOCV.indOp1Fenetre=-1;
						pOCV.indOp2Fenetre=-1;
						if (pOCV.opUnaireSelec) // L'op�ratuer 1 est initialis�
                            {
                            if (indOp>0) // Si une op�ration a d�j� �t� effectu�e
							    pOCV.op1=im[indOp-1][0];
						    else
							    {
                                pOCV.op1=imIni;
                                effaceImage[imIni]=false;
                                }
                            }
                        else if (pOCV.opBinaireSelec )
                            {
                            if (indOp>0 ) // Si une op�ration a d�j� �t� effectu�e
							    if (im[indOp-1]==NULL)
									pOCV.op2=NULL;
								else
									pOCV.op2=im[indOp-1][0];
						    else
							    pOCV.op2=imIni;
                            pOCV.op1=imPre;
                           }
                        im[indOp]=pOCV.ExecuterOperation();
						if (im[indOp])
							if(im[indOp][0]!=pOCV.op1 )
								{
                                if (effaceImage.find(im[indOp][0])==effaceImage.end())
                                    effaceImage[im[indOp][0]]=true;
                                }
							else
								effaceImage[im[indOp][0]]=false;
/*						else if (im[indOp])
							effaceImage[im[indOp][0]]=false;*/
						if (pOCV.opBinaireSelec && pOCV.opVideo)
                            {
                            if (seqOp.size()>1 )
                                {
                                if (imPre)
									effaceImage[imPre]=true;
								swap(imPre,pOCV.op2);
                                if (im[indOp])
                                    effaceImage[im[indOp][0]]=true;
                                }
                            else // Il n'y qu'une seule op�ration avec en argument imPre et imIni
                                {
								if (pOCV.op2==imIni && imPre==NULL)
									imPre=new ImageInfoCV(frame.rows,frame.cols,frame.flags);
								swap(imPre,imIni);

                                effaceImage[imPre]=false;
                                }				
                            }
                        indOp++;							
						}
					}
					if (indOp>0 && im[indOp-1])
						{
						if (!parent)
							break;
						wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);

						if (imPre)
                            {
                            imAcq->CloneStat(imPre);
							imAcq->DeplacerFlotOptique(imPre);
                            imPre->copyTo(frame);
                            if (frame.channels()==3 || imPre->channels()==3)
                                cout<<"Ce n'est pas normal!";
                             }
                        else
                            {
                            imAcq->CloneStat(im[indOp-1][0]);
						    im[indOp-1][0]->copyTo(frame);
                            if (frame.channels()==3 || im[indOp-1][0]->channels()==3)
                                cout<<"Ce n'est pas normal!";
                            }
                        std::map<ImageInfoCV*,bool>::iterator it;
						for (it=effaceImage.begin();it!=effaceImage.end();it++)
							if (it->second)
								{
                                delete it->first;
                                }
						effaceImage.clear();
						}
                    delete []im;
					}
                else
                    cout<<"erreur";
				if (!modeMoyenne)	// Pas de filtrage Butterworth
					{
					if (!parent)
						break;
					wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);

					frame.copyTo((*((Mat *)imAcq))); // get a new frame from camera
                            if (frame.channels()==3 )
                                cout<<"Ce n'est pas normal!";
					}
				else
					{
					{
					if (!parent)
						break;
					wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);
					frame.copyTo((*((Mat *)imAcq))); // get a new frame from camera
					for (int i=0;i<frame.rows;i++)
						{
						unsigned char *val=frame.ptr(i);
						unsigned char *valB1=frame1.ptr(i);
						unsigned char *valB2=frame2.ptr(i);
						for (int j=0;j<frame.cols;j++)
							for (int k=0;k<frame.channels();k++,valB1++,valB2++,val++)
								*val = bbButter[indFiltreMoyenne]*(*valB1 + *valB2)-aaButter[indFiltreMoyenne]* *val;
						
						}
					}
					frame1.copyTo(frame2);
					frame.copyTo(frame1);
					frame.copyTo((*((Mat *)imAcq))); // get a new frame from camera
					}

				}
			if (parent)
				{
				EvtPointSuivis *x= new EvtPointSuivis(VAL_EVT_PTS_SUIVIS);
				x->ptId=repereIni;
				x->ptApp=repere;
				x->SetTimestamp(wxGetUTCTimeMillis().GetLo());
				x->indEvt=indEvt++;
				//wxQueueEvent( ((FenetrePrincipale*)parent)->GetEventHandler(), x);
				if (!parent)
					break;
				bool attendre=true;
				int nbBoucle=0;
				while(attendre && nbBoucle<10)
					{
					{
					if (!parent)
						break;
					wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);
					if (((FenetrePrincipale*)parent)->IndEvtCam()+1==indEvt || nbBoucle==9)
						{
						wxQueueEvent( ((FenetrePrincipale*)parent)->GetEventHandler(), x);
						attendre=false;
						}
					}
					nbBoucle++;
					this->Sleep(10);
					}
				}
			else
				break;
			if (TestDestroy())
				break;
			}
		}
	delete captureVideo;
    delete imIni;
    delete imPre;
	}
captureVideo=NULL;

return (wxThread::ExitCode)0;  
}

wxThread::ExitCode CameraOpenCV::Acquisition32BitsFloatRGB()
{
#ifdef _FLUXHTTP__
captureVideo = new cv::VideoCapture("192.168.0.1:8080"); 
#else
captureVideo= new VideoCapture(indId); 
#endif
if (captureVideo->isOpened())
	{
	Mat frameFlt1;
	Mat frameFlt2;
	Mat frame;
	Mat frameFlt;
	std::vector<cv::Point2f> repereIni,repere;
	while (!captureVideo->retrieve(frame));
	frame.convertTo(frameFlt2,CV_32FC3);
	while (!captureVideo->retrieve(frame));
	frame.convertTo(frameFlt1,CV_32FC3);

	for(;true;)
		{
		if (captureVideo->grab()) // get a new frame from camera
			{
			captureVideo->retrieve(frame);
			if (parent)
				{


				if (!modeMoyenne)	// Pas de filtrage Butterworth
					{
					wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);
					frame.convertTo(frameFlt,CV_32FC3);
					(*((Mat *)imAcq)) =frameFlt; // get a new frame from camera
					}
				else
					{
					{
					wxCriticalSectionLocker enter(((FenetrePrincipale*)parent)->travailCam);
					frame.convertTo(frameFlt,CV_32FC3);

					(*((Mat *)imAcq)) =frameFlt; // get a new frame from camera
					for (int i=0;i<frame.rows;i++)
						{
						float *val=(float *)frameFlt.ptr(i);
						float *valB1=(float *)frameFlt1.ptr(i);
						float *valB2=(float *)frameFlt2.ptr(i);
						for (int j=0;j<frame.cols;j++)
							for (int k=0;k<frame.channels();k++,valB1++,valB2++,val++)
								*val = bbButter[indFiltreMoyenne]*(*valB1 + *valB2)-aaButter[indFiltreMoyenne]* *val;
						
						}
					}
					frameFlt1.copyTo(frameFlt2);
					frameFlt.copyTo(frameFlt1);
					}

				}
			if (parent)
				{
				EvtPointSuivis *x= new EvtPointSuivis(VAL_EVT_PTS_SUIVIS);
				x->ptId=repereIni;
				x->ptApp=repere;
				x->SetTimestamp(wxGetUTCTimeMillis().GetLo());
				wxQueueEvent( ((FenetrePrincipale*)parent)->GetEventHandler(), x);

				this->Sleep(tpsInactif);
				}
			else
				break;
			if (TestDestroy())
				break;
			}
		}
	delete captureVideo;
	}
captureVideo=NULL;

return (wxThread::ExitCode)0;  
}


wxThread::ExitCode CameraOpenCV::Entry()
{
if (captureVideo==NULL)
	{
	captureVideo = new cv::VideoCapture(indId); 
	if(!captureVideo || !captureVideo->isOpened())
		return (wxThread::ExitCode)-1;
	}
if (!fluxOuvert &&(indId<0 || indId>=NBCAMERA))
	return (wxThread::ExitCode)-1; 	
if (typeAcq==CV_8UC3 || fluxOuvert)
	return Acquisition8BitsRGB();	
else if (typeAcq==CV_32FC3)
	return Acquisition32BitsFloatRGB();	
return (wxThread::ExitCode)0;  
}

void FenetrePrincipale::OnThreadUpdateQueue(EvtPointSuivis &w)
{

if (cam && (cam->IsRunning() || cam->IsPaused()))
	{
	long x=wxGetUTCTimeMillis().GetLo();
//	if (x<10)
	{
	wxCriticalSectionLocker enter(travailCam);

	delete feuille->BitmapAffichee();
	feuille->BitmapAffichee(NULL);
	MAJNouvelleImage();
	}
	indEvtCam=w.indEvt;
	if (tpsPreEvt>=-1)
		{
		tpsPreEvt++;
		if (tpsPreEvt==10)
			{
			x=wxGetUTCTimeMillis().GetLo()-x;
			long t=cam->DefTpsInactif();
			t= max(x,t);
			wxCriticalSectionLocker enter(travailCam);
			cam->DefTpsInactif(x+10);
			tpsPreEvt=-2;
		}
		}
	}
return;
}

void FenetrePrincipale::DoPauseThread()
{    
if (cam==NULL)
	return;
// anytime we access the m_pThread pointer we must ensure that it won't    
// be modified in the meanwhile; since only a single thread may be    
// inside a given critical section at a given time, the following code    
// is safe:    
wxCriticalSectionLocker enter(travailCam);    
// without a critical section, once reached this point it may happen        
// that the OS scheduler gives control to the MyThread::Entry() function,        
// which in turn may return (because it completes its work) making        
// invalid the m_pThread pointer        
if (cam->Pause() != wxTHREAD_NO_ERROR )            
	wxLogError("Can't pause the thread!");    
}

void FenetrePrincipale::OnCloseThread(wxCloseEvent&)
{    
wxCriticalSectionLocker enter(travailCam);

if (cam)         // does the thread still exist?        
	{            
	cam->parent=NULL;
	wxMessageOutputDebug().Printf("MYFRAME: deleting thread");            
	if (cam->IsRunning())
		{
		if (cam->Delete() != wxTHREAD_NO_ERROR )                
			wxLogError("Can't delete the thread!");        
		}
	cam =NULL;	
	}    
 // exit from the critical section to give the thread            
 // the possibility to enter its destructor            
 // (which is guarded with m_pThreadCS critical section!)    
 }

void FenetrePrincipale::DefSeqCamera(std::vector <ParametreOperation> *s)
{  
if (!cam)
	return;

seqOp= *s;
cam->seqActualisee=true;
wxCriticalSectionLocker enter(paramCam);

cam->seqOp=*s;
}

void FenetrePrincipale::RazSeqOp()
{
seqOp.clear();
if (!cam)
	return;

wxCriticalSectionLocker enter(paramCam);

cam->seqOp.clear();
};
