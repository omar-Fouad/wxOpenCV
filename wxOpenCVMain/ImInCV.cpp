// Modif du 6/05/08 #include "\librairies\include\Tiff.h"
#include "ImageInfo.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
using namespace std;

int ImageInfoCV::indOpConvolution=-1;
int ImageInfoCV::indOpMorphologie=-1;
int ImageInfoCV::typeResultat=-1;	/*< R�sultat du m�me type que les op�randes */
int ImageInfoCV::fctImage=cv::BORDER_CONSTANT;	/*< Valeur -1 pour les pixels non d�finis */
int ImageInfoCV::typeSeuillage=0;
int ImageInfoCV::tailleOperateur=0;
int ImageInfoCV::centrageResultat=0;
int ImageInfoCV::typeOndelette=0;
int ImageInfoCV::tailleOndelette=0;
int ImageInfoCV::nbIterOperateur=0;




char *ImageInfoCV::tpFct[9]={"simple","tore","sym�trique","libre en z","libre en x","libre en y","libre ne xy","libre en xz","libre en yz"};
static char *tpSeuil[]={"variance","entropie","moment","local","manuel","hysteresis"};
static char *tlOp[]={"3x3","5x5","7x7"};
static char *tpCt[]={"brut","seuill�","normalis�"};
static char *tpOn[]={"daubechie"};
static long tailleOnd[]={4,12,20};


void ImageInfoCV::InitImageInfo(void *eTiff)
{
InitOp();
pano=NULL;
flotOptique=NULL;
masqueMOG=NULL;
masqueOperateur = UMat();
orient=NULL;
segmvt=NULL;
/*pOCVHoughLigne=NULL;
pOCVHoughLigneProba=NULL;
pOCVHoughCercle=NULL;
pOCVBonCoin=NULL;
pOCVLucasKanade=NULL;
pOCVGunnarFarneback=NULL;
pOCVPhaseCorrelate=NULL;pOCVCalcMotionGradient = NULL;
pOCVSegmentMotion = NULL;
*/
pOCVUpdateMotionHistory=NULL;

ponderation=NULL;
silh=NULL;

#ifdef _INFOTEXTE__
nbChampGonfle=0;
gonfle = NULL;
nbChampComposite=0;
composite=NULL;
nbChampMicroscope=0;
microscope=NULL;
nbChampCamera=0;
camera=NULL;
nbChampScanner=0;
scanner=NULL;
nbChampCopolymere = 0;
copolymere = NULL;
simulation=NULL;
nbChampSimulation=0;

nomImage=NULL;				// Nom de l'image sur le disque
natureImage=NULL;			// M�thode de saisie de l'image
time_t d;

if (time(&d)!=-1)
	{
	strcpy(dateCreation,asctime(localtime(&d)));
	}
createur = NULL;			// Programme et(?) utilisateur
resolX = -1;				// Pixel par cm
resolY = -1;				// Pixel par cm 
typeMateriel=NULL;			// Nom de la chaine d'acquisition
marqueMateriel=NULL;			// Nom de la chaine d'acquisition
nomPgm=NULL;				// Nom et version du pgm 
nomProjet = NULL;
	{
	DefNomImage(NULL);
	DefNatureImage(NULL);
	DefDateCreation(NULL);
	DefMarqueMateriel("Noyau");
	DefTypeMateriel("Image Num�rique");
	DefCopyright("Noyau Version 1.0");
	DefOrdinateur("P300 SNM98");
	DefCreateur("L. Berger");
	DefDescription("Ind�finie");
	DefNomProjet("Divers");
	DefUniteZ("Ind�finie");
	DefResolZ(1);
	DefResolX(1);
	DefResolY(1);
	DefChampUtil("Inconnu",0);
	DefChampUtil("Inconnu",1);
	DefChampUtil("Inconnu",2);
	DefChampUtil("Inconnu",3);
	DefChampUtil("Inconnu",4);
	DefNomPgm(NULL);
	}
#endif
converCplxEnt = 1;

}

void ImageInfoCV::DiffusionMPI(void)
{
}


ImageInfoCV::ImageInfoCV(void):moment(NULL),statComposante(NULL),centreGComposante(NULL),locMin(NULL),locMax(NULL),contours(NULL),arbreContour(NULL)
{
eSauver=NULL;
InitImageInfo();
}

ImageInfoCV::~ImageInfoCV(void)
{
#ifdef __INFOTEXTE__
delete nomPgm;				// Nom et version du pgm
for (int i=0;i<4;i++)
	delete champUtil[i];
delete nomProjet;
delete description;
delete createur;
delete ordinateur;
delete copyright;
delete typeMateriel;
delete marqueMateriel;
delete natureImage;			// M�thode de saisie de l'image
delete nomImage;			// Nom de l'image sur le disque
delete uniteZ;
nomPgm =NULL;				// Nom et version du pgm
nomProjet =NULL;	
description =NULL;	
createur =NULL;	
ordinateur =NULL;	
copyright =NULL;	
typeMateriel =NULL;	
marqueMateriel =NULL;	
natureImage =NULL;				// M�thode de saisie de l'image
nomImage =NULL;				// Nom de l'image sur le disque
uniteZ=NULL;
#endif
/*
delete pOCVHoughLigne;
delete pOCVHoughLigneProba;
delete pOCVHoughCercle;
delete pOCVBonCoin;
delete pOCVLucasKanade;
delete pOCVGunnarFarneback;
delete pOCVCalcMotionGradient;
delete pOCVSegmentMotion;*/
delete pOCVUpdateMotionHistory;

delete ponderation;
delete silh;
delete masqueMOG;
delete orient;
delete segmvt;


}

void ImageInfoCV::MajMasque(bool actif, cv::Rect r,int ind)
{
    if (!actif && r.area() == 0)
    {
        masqueMat = cv::Mat();
        masqueOperateur = UMat();
    }
    else
    {
        if (masqueMat.rows == 0)
            masqueMat = cv::Mat::zeros(rows, cols, CV_8UC1);
        for (int i = r.y; i <= r.y + r.height &&i<rows; i++)
        {
            unsigned char *d = (unsigned char*)masqueMat.ptr(i)+r.x;
            for (int j = r.x; j <= r.x + r.width&& j<cols; j++, d++)
            {

                if (actif)
                    *d = ind;
                else
                    *d = 0;
            }
        }
        masqueMat.copyTo(masqueOperateur);
    }
    
    }

void ImageInfoCV::MajMasque(bool actif, cv::Point p, int ind)
{
    if (!actif)
    {
        masqueMat = cv::Mat();
        masqueOperateur = UMat();
    }
    else
    {
        if (masqueMat.rows == 0)
            masqueMat = cv::Mat::zeros(rows, cols, CV_8UC1);
            if (actif)
                masqueMat.at<uchar>(p)=ind;
            else
                masqueMat.at<uchar>(p) = 0;
        masqueMat.copyTo(masqueOperateur);
    }

}


void ImageInfoCV::CloneStat(ImageInfoCV *im)
{
if (im->RegionMvt()->size() != 0)
	regionsMvt = *(im->RegionMvt());
if (im->Angle()->size() != 0)
	angle = *(im->Angle());
if ((*(im->PtContours())).size()!=0)
	{
	if (contours.size()==0)
		contours.resize(channels()); 
	for (int i=0;i<channels()&&i<im->channels();i++)
		{
		contours[i].clear();
		contours[i]= (*im->PtContours())[i];
		}
	}
if ((*(im->StatComposante())).size()!=0)
	{
	if (statComposante.size()==0)
		{
		statComposante.resize(channels()); 
		centreGComposante .resize(channels()); 
		}
	for (int i=0;i<channels()&&i<im->channels();i++)
		{
		statComposante[i] = (*im->StatComposante())[i];
		centreGComposante[i] = (*im->CentreGComposante())[i];
		}
	}
if (im->MomentComposante()->size()!=0)
	{
		{
		moment.resize(channels());
		huMoment.resize(channels());
		}
	for (int i=0;i<channels()&&i<im->channels();i++)
		{
		moment[i] = (*im->MomentComposante())[i];
		huMoment[i] = (*im->HuMoment())[i];
		}

	}
if ((*(im->HoughCercle())).size()!=0)
	{
	if (cercle.size()==0)
		cercle.resize(channels());
	for (int i=0;i<channels()&&i<im->channels();i++)
		cercle[i] = (*im->HoughCercle())[i];

	}
if ((*(im->HoughLigne())).size()!=0)
	{
	if (ligne.size()==0)
		ligne.resize(channels());
	for (int i=0;i<channels()&&i<im->channels();i++)
		ligne[i] = (*im->HoughLigne())[i];
	}
if ((*(im->HoughLigneProba())).size()!=0)
	{
	if (ligneP.size()==0)
		ligneP.resize(channels());
	for (int i=0;i<channels()&&i<im->channels();i++)
		ligneP[i] = (*im->HoughLigneProba())[i];
	}
std::map<int, std::vector<cv::KeyPoint> >::iterator it = im->pointCle.begin();
pointCle.clear();
for (; it != im->pointCle.end(); it++)
    pointCle.insert(make_pair(it->first, it->second));

pointCleApp.clear();
for (it = im->pointCleApp.begin(); it != im->pointCleApp.end(); it++)
    pointCleApp.insert(make_pair(it->first, it->second));

descripteur.clear();
std::map<int, cv::Mat >::iterator itMat = im->descripteur.begin();
for (itMat = im->descripteur.begin(); itMat != im->descripteur.end(); itMat++)
    descripteur.insert(make_pair(itMat->first, itMat->second));

if (im->PointCleMSER()->size() != 0)
    kMser = *im->PointCleMSER();
matches.clear();
std::map<int, std::vector<cv::DMatch > >::iterator itMatch = im->matches.begin();
for (itMatch = im->matches.begin(); itMatch != im->matches.end(); itMatch++)
    matches.insert(make_pair(itMatch->first, itMatch->second));
if (im->BonCoin()->size()!=0)
	{
	if (boncoin.size()==0)
		boncoin.resize(channels());
	for (int i=0;i<channels()&&i<im->channels();i++)
		boncoin[i] = (*im->BonCoin())[i];
	}
if (im->CoinRef()->size()!=0)
	{
	if (coinRef.size()==0)
		coinRef.resize(channels());
	for (int i=0;i<channels()&&i<im->channels();i++)
		coinRef[i] = (*im->CoinRef())[i];
	}
if (im->CaffeResultPret())
{
    probCaffe = im->probCaffe.clone();
    probRetenue.clear();
    for (int i = 0; i < im->probRetenue.size(); i++)
        probRetenue.push_back(im->probRetenue[i]);
    nomClassseCaffe.clear();
    for (int i = 0; i < im->nomClassseCaffe.size(); i++)
        nomClassseCaffe.push_back(im->nomClassseCaffe[i]);

}
if (im->YoloResultPret())
{
    objetsYolo = im->objetsYolo.clone();
    rectYolo.clear();
    for (int i = 0; i < im->rectYolo.size(); i++)
        rectYolo.push_back(im->rectYolo[i]);
    nomClassseYolo.clear();
    for (int i = 0; i < im->nomClassseYolo.size(); i++)
        nomClassseYolo.push_back(im->nomClassseYolo[i]);

}


 }

void ImageInfoCV:: DeplacerFlotOptique(ImageInfoCV *im)/*<! Fonction d�pla�ant le pointeur flotOptique de im dans this. Celui de im devient nul */ 
{
if (!im->FlotOptique())
	return;
if (im->channels()!=channels())
	return;
delete []flotOptique;
flotOptique=im->FlotOptique();
//im->RazFlotOptique();
}


int ImageInfoCV::EtapeOp()
{
int m=-1;
map<string,ParametreOperation>::iterator it;
for (it=listeOpAttribut.begin();it!=listeOpAttribut.end();it++)
	if (it->second.indEtape>m)
		m = it->second.indEtape;
return m;


}



ImageInfoCV::ImageInfoCV(void *r):cv::UMat(1,1,3)	
//			((EnTeteTiff*)r)->LitNbLigne(),
//			((EnTeteTiff*)r)->LitNbColonne(),
//			((EnTeteTiff*)r)->LitNbPlan())
{
eSauver=NULL;
InitImageInfo(r);
}


ImageInfoCV::ImageInfoCV(long nbL,long nbC,int nbP):cv::UMat(nbL, nbC, nbP)
{
eSauver=NULL;
InitImageInfo();
}

ParametreOperation *ImageInfoCV::AjoutOpAttribut(ParametreOperation *p)
{
listeOpAttribut[p->nomOperation]=*p;
return p;
}


std::vector<cv::DMatch> *ImageInfoCV::Appariement(char type)
{
if  (type!=-1)
    if (matches.find(type) != matches.end())
        return &matches[type];
    else
        return NULL;
else
{
    if (matches.begin()!= matches.end())
        return &matches.begin()->second;
    }
return NULL;
}

cv::Mat *ImageInfoCV::Descripteur(char type)
{ 
if  (type!=-1)
    if (descripteur.find(type) != descripteur.end())
        return &descripteur[type];
    else
        return NULL;
else
{
    if (descripteur.begin()!= descripteur.end())
        return &descripteur.begin()->second;
    }
return NULL;
};

std::vector<cv::KeyPoint> *ImageInfoCV::PointCle(char type)
{
if  (type!=-1)
    if (pointCle.find(type) != pointCle.end())
        return &pointCle[type];
    else
        return NULL;
else
{
    if (pointCle.begin()!= pointCle.end())
        return &pointCle.begin()->second;
    }
return NULL;
}

std::vector<cv::KeyPoint> *ImageInfoCV::PointCleApp(char type)
{
if  (type!=-1)
    if (pointCleApp.find(type) != pointCleApp.end())
        return &pointCleApp[type];
    else
        return NULL;
else
{
    if (pointCleApp.begin()!= pointCleApp.end())
        return &pointCleApp.begin()->second;
    }
return NULL;
}

/*
ParametreOperation *ImageInfoCV::ParamOCVHoughLigne(ParametreOperation *p)
{
if (p)
	{
	if (pOCVHoughLigne==NULL)
		pOCVHoughLigne =  new ParametreOperation;
	*pOCVHoughLigne=*p;
	}
return pOCVHoughLigne;
}
	
ParametreOperation *ImageInfoCV::ParamOCVHoughLigneProba(ParametreOperation *p)
{
if (p)
	{
	if (pOCVHoughLigneProba==NULL)
		pOCVHoughLigneProba =  new ParametreOperation;
	*pOCVHoughLigneProba=*p;
	}
return pOCVHoughLigneProba;
}

ParametreOperation *ImageInfoCV::ParamOCVHoughCercle(ParametreOperation *p)
{
if (p)
	{
	if (pOCVHoughCercle==NULL)
		pOCVHoughCercle =  new ParametreOperation;
	*pOCVHoughCercle=*p;
	}
return pOCVHoughCercle;
}

ParametreOperation *ImageInfoCV::ParamOCVBonCoin(ParametreOperation *p)
{
if (p)
	{
	if (pOCVBonCoin==NULL)
		pOCVBonCoin =  new ParametreOperation;
	*pOCVBonCoin=*p;
	}
return pOCVBonCoin;
}

ParametreOperation *ImageInfoCV::ParamOCVLucasKanade(ParametreOperation *p)
{
if (p)
	{
	if (pOCVLucasKanade==NULL)
		pOCVLucasKanade =  new ParametreOperation;
	*pOCVLucasKanade=*p;
	}
return pOCVLucasKanade;
}
ParametreOperation *ImageInfoCV::ParamOCVGunnarFarneback(ParametreOperation *p)
{
	if (p)
	{
		if (pOCVGunnarFarneback == NULL)
			pOCVGunnarFarneback = new ParametreOperation;
		*pOCVGunnarFarneback = *p;
	}
	return pOCVGunnarFarneback;
}

ParametreOperation *ImageInfoCV::ParamOCVPhaseCorrelate(ParametreOperation *p)
{
	if (p)
	{
		if (pOCVPhaseCorrelate == NULL)
			pOCVPhaseCorrelate = new ParametreOperation;
		*pOCVPhaseCorrelate = *p;
	}
	return pOCVPhaseCorrelate;
}


ParametreOperation *ImageInfoCV::ParamOCVCalcMotionGradient(ParametreOperation *p)
{
	if (p)
	{
		if (pOCVCalcMotionGradient == NULL)
			pOCVCalcMotionGradient = new ParametreOperation;
		*pOCVCalcMotionGradient = *p;
	}
	return pOCVCalcMotionGradient;
}
ParametreOperation *ImageInfoCV::ParamOCVSegmentMotion(ParametreOperation *p)
{
	if (p)
	{
		if (pOCVSegmentMotion == NULL)
			pOCVSegmentMotion = new ParametreOperation;
		*pOCVSegmentMotion = *p;
	}
	return pOCVSegmentMotion;
}*/

ParametreOperation *ImageInfoCV::ParamOCVUpdateMotionHistory(ParametreOperation *p)
{
	if (p)
	{
		if (pOCVUpdateMotionHistory == NULL)
			pOCVUpdateMotionHistory = new ParametreOperation;
		*pOCVUpdateMotionHistory = *p;
	}
	return pOCVUpdateMotionHistory;
}


void ImageInfoCV::DefProprieteImage(char *nomFichier)
{
}



void ImageInfoCV::SauverProprieteProjet(char *nomFichier)
{
}


std::string ImageInfoCV::InfoImage(void)
{
    string texte;
    switch (depth())
    {
    case CV_32S:
        texte = "int (32 bits)";
        break;
    case CV_16U:
        texte = "unsigned short (16 bits)";
        break;
    case CV_16S:
        texte = " short (16 bits)";
        break;
    case CV_8U:
        texte = "Unsigned char (8 bits)";
        break;
    case CV_8S:
        texte = "Signed char (8 bits)";
        break;
    case CV_32F:
        texte = "Single precision floating point (32 bits) by channel";
        break;
    case CV_64F:
        texte = "double precision floating point (64 bits) by channel";
        break;
    default:
        texte = "Unknown size!";
    }

    texte = texte +"\n";
    for (int i = 0; i<channels() ; i++)
    {
        texte += "(Min,Max) Channels " + to_string(i);
        texte += " = ( " + std::to_string(static_cast<double>((*(MinIm()))[i]));
        texte += " , " ;
        texte += std::to_string(static_cast<double>((*(MaxIm()))[i])) + ")\n";
    }
    return texte;


}


void	ImageInfoCV::DefFctImage(char* c)
{
long nb=strlen(c);
char *a=new char[nb+1];
long i=0;

a[nb]='\0';
while(i<nb)
	{
	a[i] = tolower(c[i]);
	i++;
	}
i = 0;
while (	i<4 && strcmp(a,tpFct[i]))
	i++;
if (i<4)
	{	
	fctImage = i;
//	DefTypeFctImage(fctImage);
	}
}


void	ImageInfoCV::DefTypeSeuillage(char* c)
{
long nb=strlen(c);
char *a=new char[nb+1];
long i=0;

a[nb]='\0';
while(i<nb)
	{
	a[i] = tolower(c[i]);
	i++;
	}
i = 0;
while (	i<6 && strcmp(a,tpSeuil[i]))
	i++;
if (i<6)	
	typeSeuillage = i;
}

void	ImageInfoCV::DefTailleOperateur(char* c)
{
long nb=strlen(c);
char *a=new char[nb+1];
long i=0;

a[nb]='\0';
while(i<nb)
	{
	a[i] = tolower(c[i]);
	i++;
	}
i = 0;
while (	i<3 && strcmp(a,tlOp[i]))
	i++;
if (i<3)	
	tailleOperateur= i;
}

void	ImageInfoCV::DefCentrageResultat(char* c)
{
long nb=strlen(c);
char *a=new char[nb+1];
long i=0;

a[nb]='\0';
while(i<nb)
	{
	a[i] = tolower(c[i]);
	i++;
	}
i = 0;
while (	i<3 && strcmp(a,tpCt[i]))
	i++;
if (i<3)
	{	
	centrageResultat= i;
//	DefDomaineResultat(centrageResultat);
	}
}

void	ImageInfoCV::DefTypeOndelette(char *c)
{
long nb=strlen(c);
char *a=new char[nb+1];
long i=0;

a[nb]='\0';
while(i<nb)
	{
	a[i] = tolower(c[i]);
	i++;
	}
i = 0;
while (	i<1 && strcmp(a,tpOn[i]))
	i++;
if (i<1)	
	typeOndelette= i;
}

void	ImageInfoCV::DefTailleOndelette(long n)
{

if (n<3)
	tailleOndelette = n;	
}

void	ImageInfoCV::DefNbIterOperateur(long n)
{

nbIterOperateur = n;	
}

void ImageInfoCV::DefConverCplxEnt(char c)
{
converCplxEnt = c;
}

char *ImageInfoCV::LitFctImage(void)
{
return (tpFct[fctImage]);
}

char *ImageInfoCV::LitTypeSeuillage(void)
{
return(tpSeuil[typeSeuillage]);
}

char *ImageInfoCV::LitTailleOperateur(void)
{
return(tlOp[tailleOperateur]);
}

char *ImageInfoCV::LitCentrageResultat(void)
{
return(tpCt[centrageResultat]);
}

char ImageInfoCV::LitValCentrageResultat(void)
{
return((char)centrageResultat);
}

char *ImageInfoCV::LitTypeOndelette(void)
{
return(tpOn[typeOndelette]);
}

long ImageInfoCV::LitTailleOndelette(void)
{
return(tailleOndelette);
}

long ImageInfoCV::LitNbIterOperateur(void)
{
return(nbIterOperateur);
}

char ImageInfoCV::LitConverCplxEnt(void)
{
return(converCplxEnt);
}

char *ImageInfoCV::InfoPixel(long x,long y,long xp,long yp)
{
return 0;
}






//	********* D�finition des membres priv�s
void 	ImageInfoCV::DefNomImage(char *c)
{
#ifdef __INFOTEXTE__
if (c==NULL)
	c = "Sans Titre";
long	n =strlen(c)+1;
nomImage = new char[n];
if (nomImage==NULL)
	{
	return;
	}
strcpy(nomImage,c);
return;
#endif
}


void 	ImageInfoCV::DefNatureImage(char *c)
{
#ifdef __INFOTEXTE__
if (c==NULL)
	c = "Inconnue";
long	n =strlen(c)+1;

natureImage = new char[n];
if (natureImage==NULL)
	{
	return;
	}
strcpy(natureImage,c);
return;
#endif
}


void 	ImageInfoCV::DefDateCreation(char *t)
{
#ifdef __INFOTEXTE__
  if (t==NULL)
	   return;
strcpy(dateCreation,t);
#endif
}


void 	ImageInfoCV::DefCreateur(char *c)
{
#ifdef __INFOTEXTE__
if (c==NULL)
	{
	createur = new char[10];
	strcpy(createur,"Op�rateur");
	return;
	}
long	n =strlen(c)+1;

createur = new char[n];
if (createur==NULL)
	{
	return;
	}
strcpy(createur,c);
return;
#endif
}

void 	ImageInfoCV::DefDescription(char *c)
{
#ifdef __INFOTEXTE__
if (c==NULL)
	{
	description = new char[10];
	strcpy(description,"Ind�finie");
	return;
	}
long	n =strlen(c)+1;

description = new char[n];
if (description==NULL)
	{
	return;
	}
strcpy(description,c);
return;
#endif
}

void 	ImageInfoCV::DefUniteZ(char *c)
{
#ifdef __INFOTEXTE__
if (c==NULL)
	{
	uniteZ = new char[10];
	strcpy(uniteZ,"Ind�finie");
	return;
	}
long	n =strlen(c)+1;

uniteZ = new char[n];
if (uniteZ==NULL)
	{
	return;
	}
strcpy(uniteZ,c);
return;
#endif
}

void	ImageInfoCV::DefChampCopolymere(long indChamp,char *c)
{
#ifdef __INFOTEXTE__
DefChampMultiple(indChamp,c,&copolymere,nbChampCopolymere);
return;
#endif
}

void 	ImageInfoCV::DefChampCopolymere(char *champ)
{
#ifdef __INFOTEXTE__
copolymere = DefChampMultiple(champ,nbChampCopolymere);
return;
#endif
}

void	ImageInfoCV::DefChampComposite(long indChamp,char *c)
{
#ifdef __INFOTEXTE__
DefChampMultiple(indChamp,c,&composite,nbChampComposite);
return;
#endif
}

void 	ImageInfoCV::DefChampComposite(char *champ)
{
#ifdef __INFOTEXTE__
composite = DefChampMultiple(champ,nbChampComposite);
return;
#endif
}

void	ImageInfoCV::DefChampGonfle(long indChamp,char *c)
{
#ifdef __INFOTEXTE__
DefChampMultiple(indChamp,c,&copolymere,nbChampGonfle);
return;
#endif
}

void 	ImageInfoCV::DefChampGonfle(char *champ)
{
#ifdef __INFOTEXTE__
gonfle = DefChampMultiple(champ,nbChampGonfle);
return;
#endif
}

void	ImageInfoCV::DefChampMicroscope(long indChamp,char *c)
{
#ifdef __INFOTEXTE__
DefChampMultiple(indChamp,c,&microscope,nbChampMicroscope);
return;
#endif
}

void	ImageInfoCV::DefChampSimulation(long indChamp,char *c)
{
#ifdef __INFOTEXTE__
DefChampMultiple(indChamp,c,&simulation,nbChampSimulation);
return;
#endif
}

void 	ImageInfoCV::DefChampSimulation(char *champ)
{
#ifdef __INFOTEXTE__
simulation = DefChampMultiple(champ,nbChampSimulation);
return;
#endif
}

void 	ImageInfoCV::DefChampMicroscope(char *champ)
{
#ifdef __INFOTEXTE__
microscope = DefChampMultiple(champ,nbChampMicroscope);
return;
#endif
}

void	ImageInfoCV::DefChampScanner(long indChamp,char *c)
{
#ifdef __INFOTEXTE__
DefChampMultiple(indChamp,c,&scanner,nbChampScanner);
return;
#endif
}

void 	ImageInfoCV::DefChampScanner(char *champ)
{
#ifdef __INFOTEXTE__
scanner = DefChampMultiple(champ,nbChampScanner);
return;
#endif
}

void	ImageInfoCV::DefChampCamera(long indChamp,char *c)
{
#ifdef __INFOTEXTE__
DefChampMultiple(indChamp,c,&camera,nbChampCamera);
return;
#endif
}

void 	ImageInfoCV::DefChampCamera(char *champ)
{
#ifdef __INFOTEXTE__
camera = DefChampMultiple(champ,nbChampCamera);
return;
#endif
}

char *ImageInfoCV::ConversionChampUtilChampTiff(char **tabChamp,long nbChamp,long &nbCaractere)
{
nbCaractere=1;
long i;
for ( i=0;i<nbChamp && tabChamp[i];i++)
	nbCaractere += strlen(tabChamp[i])+1;
static char *tmp=NULL;
static long tailleAllouee=0;
if (tailleAllouee<nbCaractere)
	{
	delete tmp;
	tmp = new char[nbCaractere];
	tailleAllouee = nbCaractere;
	}
if (!tmp)
	{
	return(NULL);
	}
char *c=tmp;
for (i=0;i<nbChamp && tabChamp[i];i++)
	{
	strcpy(c,tabChamp[i]);
	c+=strlen(tabChamp[i])+1;
	}
*c=0;
return(tmp);
}

char** 	ImageInfoCV::DefChampMultiple(char *champ,long &nbChamp)
{
char	double0=0;
long	nbC=0 ;
char	*c=champ;
char	**tabChamp;

if( !c) 
	return NULL;
while (!double0)
	if (*c!=0)
		c++;
	else
		{
		nbC++;
		c++;
		if (*c==0)
			double0 = 1;
		c++;
		}
nbChamp = nbC;
c=champ;
tabChamp = new char*[nbChamp];
if (!tabChamp)
	{
	return(NULL);
	}
for (long i=0;i<nbChamp;i++)
	{
	long	nbC=strlen(c)+1;
	int longueurAllouee=pow(10.0,double(int(1+log(nbC+1.0)/log(10.0))));
	tabChamp[i] = new char[longueurAllouee];
	if (!tabChamp[i])
		{
		return(NULL);
		}
	strcpy(tabChamp[i],c);
	c+=nbC;
	}
return(tabChamp);
}

void	ImageInfoCV::DefChampMultiple(long indChamp,char *c,char ***tabChamp,long &nbChamp)
{
if (*tabChamp==NULL)
	{
	nbChamp =45;
	*tabChamp = new char*[nbChamp];
	if (*tabChamp==NULL)
		{
		return;
		}
	for (long i=0;i<nbChamp;i++)
		(*tabChamp)[i] = NULL;
	}
if (indChamp<0 || indChamp>=nbChamp)
	{
	return;
	}

if ((*tabChamp)[indChamp]!=NULL)
	{
	int	n=strlen((*tabChamp)[indChamp]);
	int longueurAllouee=pow(10.0,double(int(1+log(n+1.)/log(10.))));
	if (strlen(c)>longueurAllouee)
		{
		delete (*tabChamp)[indChamp];
		(*tabChamp)[indChamp] = new char [int(pow(10.0,double(int(1+log(strlen(c)+1.0)/log(10.0)))))];
		}
	}
else
		(*tabChamp)[indChamp] = new char [int(pow(10.0,double(int(1+log(strlen(c)+1.0)/log(10.0)))))];
if ((*tabChamp)[indChamp]==NULL)
	{
	return;
	}
strcpy((*tabChamp)[indChamp],c);
return;
}



void 	ImageInfoCV::DefChampUtil(char *c,long i,long nbOctets)
{
#ifdef __INFOTEXTE__
if (i>=20)
	{
	return;
	}
if (c==NULL)
	c="Inconnu";
long	n;
if (nbOctets==-1)
	n =strlen(c)+1;
else
	n = nbOctets;

champUtil[i] = new char[n];
if (champUtil[i]==NULL)
	{
	return;
	}
memcpy(champUtil[i],c,n);
return;
#endif
}



void	ImageInfoCV::DefResolX(double x)
{
#ifdef __INFOTEXTE__
resolX = x;
return;
#endif
}

void	ImageInfoCV::DefResolZ(double z)
{
#ifdef __INFOTEXTE__
resolZ = z;
return;
#endif
}


void	ImageInfoCV::DefResolY(double y)
{
#ifdef __INFOTEXTE__
resolY = y;
return;
#endif
}


void	ImageInfoCV::DefTypeMateriel(char *c)
{
#ifdef __INFOTEXTE__
if (c==NULL)
	{
	long	n =strlen("Aucun")+1;
	typeMateriel = new char[n];
	strcpy(typeMateriel,"Aucun");

	return;
	}
long	n =strlen(c)+1;

typeMateriel = new char[n];
if (typeMateriel==NULL)
	{
	return;
	}
strcpy(typeMateriel,c);
return;
#endif
}

void	ImageInfoCV::DefCopyright(char *c)
{
#ifdef __INFOTEXTE__
if (c==NULL)
	{
	long	n =strlen("Aucun")+1;
	copyright = new char[n];
	strcpy(copyright,"Aucun");

	return;
	}
long	n =strlen(c)+1;

copyright = new char[n];
if (copyright==NULL)
	{
	return;
	}
strcpy(copyright,c);
return;
#endif
}



void	ImageInfoCV::DefOrdinateur(char *c)
{
#ifdef __INFOTEXTE__
if (c==NULL)
	{
	long	n =strlen("Inconnu")+1;
	ordinateur = new char[n];
	strcpy(ordinateur,"Inconnu");

	return;
	}
long	n =strlen(c)+1;

ordinateur = new char[n];
if (ordinateur==NULL)
	{
	return;
	}
strcpy(ordinateur,c);
return;
#endif
}


void 	ImageInfoCV::DefMarqueMateriel(char *c)
{
#ifdef __INFOTEXTE__
if (c==NULL)
	{
	long	n =strlen("Aucun")+1;
	marqueMateriel = new char[n];
	strcpy(marqueMateriel,"Aucun");

	return;
	}
long	n =strlen(c)+1;

marqueMateriel = new char[n];
if (marqueMateriel==NULL)
	{
	return;
	}
strcpy(marqueMateriel,c);
return;
#endif
}


void 	ImageInfoCV::DefNomPgm(char *c)
{
#ifdef __INFOTEXTE__
if (c==NULL)
	{
	long	n =strlen("Noyau version1.0")+1;
	nomPgm = new char[n];
	strcpy(nomPgm,"Noyau version1.0");

	return;
	}
long	n =strlen(c)+1;

nomPgm = new char[n];
if (nomPgm==NULL)
	{
	return;
	}
strcpy(nomPgm,c);
return;
#endif

}

void 	ImageInfoCV::DefNomProjet(char *c)
{
#ifdef __INFOTEXTE__
if (c==NULL)
	{
	long	n =strlen("Divers")+1;
	nomProjet = new char[n];
	strcpy(nomProjet,"Divers");

	return;
	}
long	n =strlen(c)+1;

nomProjet = new char[n];
if (nomProjet==NULL)
	{
	return;
	}
strcpy(nomProjet,c);
return;

#endif
}




//	********* Lecture des membres priv�s
char* 	ImageInfoCV::LitNomImage(void)
{
#ifdef __INFOTEXTE__
if (nomImage==NULL)
	{
	nomImage= new char[strlen("Inconnu")+1];
	strcpy(nomImage,"Inconnu");
	}
//char	*c= new char[strlen(nomImage)+1];
//strcpy(c,nomImage);
return(nomImage);
#endif
return NULL;
}

char* 	ImageInfoCV::LitNatureImage(void)
{
#ifdef __INFOTEXTE__
if (natureImage==NULL)
	{
	natureImage= new char[strlen("Inconnu")+1];
	strcpy(natureImage,"Inconnu");
	}
//char	*c= new char[strlen(natureImage)+1];
//strcpy(c,natureImage);
return(natureImage);
#endif
return NULL;
}

char  	*ImageInfoCV::LitDateCreation(void)
{
#ifdef __INFOTEXTE__
return dateCreation;
#endif
return NULL;
}

char* 	ImageInfoCV::LitCreateur(void)
{
#ifdef __INFOTEXTE__

if (createur==NULL)
	{
	createur= new char[strlen("Inconnu")+1];
	strcpy(createur,"Inconnu");
	}
//char	*c= new char[strlen(createur)+1];
//strcpy(c,createur);
return(createur);
#endif
return NULL;
}

char* 	ImageInfoCV::LitDescription(void)
{
#ifdef __INFOTEXTE__

if (description==NULL)
	{
	description= new char[strlen("Inconnu")+1];
	strcpy(description,"Inconnu");
	}
//char	*c= new char[strlen(description)+1];
//strcpy(c,description);
return(description);
#endif
return NULL;
}

double	ImageInfoCV::LitResolX(void)
{
#ifdef __INFOTEXTE__
return resolX;
#endif
return NULL;
}

double	ImageInfoCV::LitResolY(void)
{
#ifdef __INFOTEXTE__
return resolY;
#endif
return NULL;
}

double	ImageInfoCV::LitResolZ(void)
{
#ifdef __INFOTEXTE__
return resolZ;
#endif
return NULL;
}

char*	ImageInfoCV::LitMarqueMateriel(void)
{
#ifdef __INFOTEXTE__
if (marqueMateriel==NULL)
	{
	marqueMateriel= new char[strlen("Inconnu")+1];
	strcpy(marqueMateriel,"Inconnu");
	}
//char	*c= new char[strlen(marqueMateriel)+1];
//strcpy(c,marqueMateriel);
return(marqueMateriel);
#endif
return NULL;
}

char*	ImageInfoCV::LitTypeMateriel(void)
{
#ifdef __INFOTEXTE__
if (typeMateriel==NULL)
	{
	typeMateriel= new char[strlen("Inconnu")+1];
	strcpy(typeMateriel,"Inconnu");
	}
//char	*c= new char[strlen(typeMateriel)+1];
//strcpy(c,typeMateriel);
return(typeMateriel);
#endif
return NULL;
}

char*	ImageInfoCV::LitCopyright(void)
{
#ifdef __INFOTEXTE__
if (copyright==NULL)
	{
	copyright= new char[strlen("Inconnu")+1];
	strcpy(copyright,"inconnu");
	}
//char	*c= new char[strlen(copyright)+1];
//strcpy(c,copyright);
return(copyright);
#endif
return NULL;
}

char*	ImageInfoCV::LitOrdinateur(void)
{
#ifdef __INFOTEXTE__
if (ordinateur==NULL)
	{
	ordinateur= new char[strlen("Inconnu")+1];
	strcpy(ordinateur,"Inconnu");
	}
//char	*c= new char[strlen(ordinateur)+1];
//strcpy(c,ordinateur);
return(ordinateur);
#endif
return NULL;
}


char*	ImageInfoCV::LitNomPgm(void)
{
#ifdef __INFOTEXTE__
if (nomPgm==NULL)
	{
	nomPgm= new char[strlen("Inconnu")+1];
	strcpy(nomPgm,"Inconnu");
	}
//char	*c= new char[strlen(nomPgm)+1];
//strcpy(c,nomPgm);
return(nomPgm);
#endif
return NULL;
}

char*	ImageInfoCV::LitNomProjet(void)
{
#ifdef __INFOTEXTE__
if (nomProjet==NULL)
	{
	nomProjet= new char[strlen("Divers")+1];
	strcpy(nomProjet,"Divers");
	}
return(nomProjet);
#endif
return NULL;
}

char*	ImageInfoCV::LitUniteZ(void)
{
#ifdef __INFOTEXTE__
if (uniteZ==NULL)
	{
	uniteZ= new char[strlen("Inconnu")+1];
	strcpy(uniteZ,"inconnu");
	}
return(uniteZ);
#endif
return NULL;
}

char*	ImageInfoCV::LitChampSimulation(long nbChamp)
{
#ifdef __INFOTEXTE__
if (simulation==NULL || (nbChamp<0 || nbChamp>=nbChampSimulation))
	{
	return(NULL);
	}
return(simulation[nbChamp]);
#endif
return NULL;
}


char*	ImageInfoCV::LitChampCopolymere(long nbChamp)
{
#ifdef __INFOTEXTE__
if (copolymere==NULL || (nbChamp<0 || nbChamp>=nbChampCopolymere))
	{
	nbChampCopolymere =0;
	return(NULL);
	}
return(copolymere[nbChamp]);
#endif
return NULL;
}

char*	ImageInfoCV::LitChampUtil(long i)
{
#ifdef __INFOTEXTE__
if (champUtil[i]==NULL)
	{
	champUtil[i]= new char[strlen("Inconnu")+1];
	strcpy(champUtil[i],"inconnu");
	}
//char	*c= new char[strlen(champUtil[i])+1];
//strcpy(c,champUtil[i]);
return(champUtil[i]);
#endif
return NULL;
}



ImageInfoCV 	*ImageInfoCV::DistObjetFond ()
{
ImageInfoCV	*result=new ImageInfoCV;
/*
Image	*op1=(Image*)this;
Image	*r=	op1->DistObjetFond();

result->DefNbColonne(r->LitNbColonne());
result->DefNbLigne(r->LitNbLigne());
result->DefNbPlan(r->LitNbPlan());
result->DefTaillePixel(r->LitTaillePixel());
result->DefTypePixel(r->LitTypePixel()|r->LitAttributPixel());
result->DefPlan(r->LitPlan());
result->DefInfoImage(r->LitInfoImage());
result->DefNbEltParPixel(r->LitNbEltParPixel());
*/
return(result);
}


ImageInfoCV 	*ImageInfoCV::Voronoi (ImageInfoCV	&z)
{
ImageInfoCV	*result=new ImageInfoCV;
/*
Image	*op1=(Image*)this,*op2=(Image*)&z;
Image	*r=	op1->Voronoi(z);

result->DefNbColonne(r->LitNbColonne());
result->DefNbLigne(r->LitNbLigne());
result->DefNbPlan(r->LitNbPlan());
result->DefTaillePixel(r->LitTaillePixel());
result->DefTypePixel(r->LitTypePixel()|r->LitAttributPixel());
result->DefPlan(r->LitPlan());
result->DefInfoImage(r->LitInfoImage());
result->DefNbEltParPixel(r->LitNbEltParPixel());
*/
return(result);
}

ImageInfoCV *ImageInfoCV::BruitImpulsionnel(double proba,long iMin,long iMax)
{
ImageInfoCV	*result=new ImageInfoCV;
/*
Image	*op1=(Image*)this;
Image	*r=	op1->BruitImpulsionnel(proba,iMin,iMax);

result->DefNbColonne(r->LitNbColonne());
result->DefNbLigne(r->LitNbLigne());
result->DefNbPlan(r->LitNbPlan());
result->DefTaillePixel(r->LitTaillePixel());
result->DefTypePixel(r->LitTypePixel()|r->LitAttributPixel());
result->DefPlan(r->LitPlan());
result->DefInfoImage(r->LitInfoImage());
result->DefNbEltParPixel(r->LitNbEltParPixel());
*/
return(result);
}

ImageInfoCV *ImageInfoCV::TransformationGeometrique(double **t, char brut)
{
ImageInfoCV	*result=new ImageInfoCV;
/*
Image	*op1=(Image*)this;
Image	*r=	op1->TransformationGeometrique(t,brut);

result->DefNbColonne(r->LitNbColonne());
result->DefNbLigne(r->LitNbLigne());
result->DefNbPlan(r->LitNbPlan());
result->DefTaillePixel(r->LitTaillePixel());
result->DefTypePixel(r->LitTypePixel()|r->LitAttributPixel());
result->DefPlan(r->LitPlan());
result->DefInfoImage(r->LitInfoImage());
result->DefNbEltParPixel(r->LitNbEltParPixel());
*/
return(result);
}



ImageInfoCV *ImageInfoCV::BruitGaussienMultiplicatif(double variance)
{
ImageInfoCV	*result=new ImageInfoCV;
/*
Image	*op1=(Image*)this;
Image	*r=	op1->BruitGaussienMultiplicatif(variance);

result->DefNbColonne(r->LitNbColonne());
result->DefNbLigne(r->LitNbLigne());
result->DefNbPlan(r->LitNbPlan());
result->DefTaillePixel(r->LitTaillePixel());
result->DefTypePixel(r->LitTypePixel()|r->LitAttributPixel());
result->DefPlan(r->LitPlan());
result->DefInfoImage(r->LitInfoImage());
result->DefNbEltParPixel(r->LitNbEltParPixel());
*/
return(result);
}


ImageInfoCV *ImageInfoCV::BruitLaplacien(double v)
{
ImageInfoCV	*result=new ImageInfoCV;
/*
Image	*op1=(Image*)this;
Image	*r=	op1->BruitLaplacien(v);

result->DefNbColonne(r->LitNbColonne());
result->DefNbLigne(r->LitNbLigne());
result->DefNbPlan(r->LitNbPlan());
result->DefTaillePixel(r->LitTaillePixel());
result->DefTypePixel(r->LitTypePixel()|r->LitAttributPixel());
result->DefPlan(r->LitPlan());
result->DefInfoImage(r->LitInfoImage());
result->DefNbEltParPixel(r->LitNbEltParPixel());
*/
return NULL;
}



void ImageInfoCV::DoEnregistrer(char **nomFic,long nbCouleur,void *palette)
{
}

/**
void ImageInfoCV::DoEnregistrer(char **nomFic,long nbCouleur,void *palette)
{
RESET_ERREUR;
if (eSauver==NULL)
	eSauver = new EnTeteTiff;
EnTeteTiff *e2=(EnTeteTiff *)eSauver;
char *r=NULL,*n=NULL;
long	i=strlen(*nomFic)-1;
while (i>=0 && (*nomFic)[i]!='.')
	i--;
if (i<0)
	{
	char *nom=new char[strlen(*nomFic)+5];
	strcpy(nom,*nomFic);
	strcat(nom,".tif");
	delete *nomFic;
	*nomFic = nom;
	}
RepertoireFichier(*nomFic,&n,&r);
DefNomImage(n);
delete n;
delete r;
e2->AjouteDescripteur(IMAGELENGTH_TIFF,SHORT_TIFF,1,LitNbLigne());
e2->AjouteDescripteur(IMAGEWIDTH_TIFF,SHORT_TIFF,1,LitNbColonne());
e2->AjouteDescripteur(PHOTOMETRICINTERPRETATION_TIFF,SHORT_TIFF,1,LitNbPlan());
e2->AjouteDescripteur(SAMPLESPARPIXEL_TIFF,SHORT_TIFF,1,LitNbPlan());
e2->AjouteDescripteur((short)TYPEPIXEL_TIFF,LONG_TIFF,1,LitTypePixel()|LitAttributPixel());
if (nbCouleur)
	e2->AjouteDescripteur(COLORMAP_TIFF,SHORT_TIFF,nbCouleur*3,(char*)palette);

if (LitNbPlan()==1)
	e2->AjouteDescripteur(BITSPERSAMPLE_TIFF,SHORT_TIFF,LitNbPlan(),LitTaillePixel());
else
	{
	short *p=new short[LitNbPlan()];
	for (long i=0;i<LitNbPlan();i++)
		p[i] =  LitTaillePixel();
	e2->AjouteDescripteur(BITSPERSAMPLE_TIFF,SHORT_TIFF,LitNbPlan(),(char *)p);
	delete p;
	}
e2->AjouteDescripteur(STRIPOFFSET_TIFF,LONG_TIFF,1,1);
e2->AjouteDescripteur(NOMBASE_TIFF,ASCII_TIFF,strlen(LitNomProjet())+1,LitNomProjet());
e2->AjouteDescripteur(SOFTWARE_TIFF,ASCII_TIFF,strlen(LitNomPgm())+1,LitNomPgm());
e2->AjouteDescripteur(DOCUMENTNAME_TIFF,ASCII_TIFF,strlen(LitNomImage())+1,LitNomImage());
e2->AjouteDescripteur(DATETIME_TIFF,ASCII_TIFF,strlen(LitDateCreation())+1,LitDateCreation());
e2->AjouteDescripteur(MAKE_TIFF,ASCII_TIFF,strlen(LitMarqueMateriel())+1,LitMarqueMateriel());
e2->AjouteDescripteur(MODEL_TIFF,ASCII_TIFF,strlen(LitTypeMateriel())+1,LitTypeMateriel());
e2->AjouteDescripteur(HOSTCOMPUTER_TIFF,ASCII_TIFF,strlen(LitOrdinateur())+1,LitOrdinateur());
e2->AjouteDescripteur(ARTIST_TIFF,ASCII_TIFF,strlen(LitCreateur())+1,LitCreateur());
e2->AjouteDescripteur(IMAGEDESCRIPTION_TIFF,ASCII_TIFF,strlen(LitDescription())+1,LitDescription());
e2->AjouteDescripteur(COPYRIGHT_TIFF,ASCII_TIFF,strlen(LitCopyright())+1,LitCopyright());

e2->AjouteDescripteur(UTILSATEUR1_TIFF,ASCII_TIFF,strlen(LitChampUtil(0))+1,LitChampUtil(0));
e2->AjouteDescripteur(UTILSATEUR2_TIFF,ASCII_TIFF,strlen(LitChampUtil(1))+1,LitChampUtil(1));
e2->AjouteDescripteur(UTILSATEUR3_TIFF,ASCII_TIFF,strlen(LitChampUtil(2))+1,LitChampUtil(2));
e2->AjouteDescripteur(UTILSATEUR4_TIFF,ASCII_TIFF,strlen(LitChampUtil(3))+1,LitChampUtil(3));
e2->AjouteDescripteur(UTILSATEUR5_TIFF,ASCII_TIFF,strlen(LitChampUtil(4))+1,LitChampUtil(4));
e2->AjouteDescripteur(RESOLUTIONUNITZ_TIFF,ASCII_TIFF,strlen(LitUniteZ())+1,LitUniteZ());
if (nbChampCopolymere)
	{
	long nbCaractere;
	char *tmp=ConversionChampUtilChampTiff(copolymere,nbChampCopolymere,nbCaractere);
	e2->AjouteDescripteur(COPOLYMERE_TIFF,ASCII_TIFF,nbCaractere,tmp);
	}
if (nbChampGonfle)
	{
	long nbCaractere;
	char *tmp=ConversionChampUtilChampTiff(gonfle,nbChampGonfle,nbCaractere);
	e2->AjouteDescripteur(GONFLE_TIFF,ASCII_TIFF,nbCaractere,tmp);
	}
if (nbChampComposite)
	{
	long nbCaractere;
	char *tmp=ConversionChampUtilChampTiff(composite,nbChampComposite,nbCaractere);
	e2->AjouteDescripteur(COMPOSITE_TIFF,ASCII_TIFF,nbCaractere,tmp);
	}
if (nbChampMicroscope)
	{
	long nbCaractere;
	char *tmp=ConversionChampUtilChampTiff(microscope,nbChampMicroscope,nbCaractere);
	e2->AjouteDescripteur(PARAM_MICROSCOPE_TIFF,ASCII_TIFF,nbCaractere,tmp);
	}
if (nbChampScanner)
	{
	long nbCaractere;
	char *tmp=ConversionChampUtilChampTiff(scanner,nbChampScanner,nbCaractere);
	e2->AjouteDescripteur(PARAM_SCANNER_TIFF,ASCII_TIFF,nbCaractere,tmp);
	}
if (nbChampCamera)
	{
	long nbCaractere;
	char *tmp=ConversionChampUtilChampTiff(camera,nbChampCamera,nbCaractere);
	e2->AjouteDescripteur(PARAM_CAMERA_TIFF,ASCII_TIFF,nbCaractere,tmp);
	}

if (nbChampSimulation)
	{
	long nbCaractere;
	char *tmp=ConversionChampUtilChampTiff(simulation,nbChampSimulation,nbCaractere);
	e2->AjouteDescripteur(SIMULATION_TIFF,ASCII_TIFF,nbCaractere,tmp);
	}



double *ext=new double[LitNbPlan()];
for (i=0;i<LitNbPlan();i++)
	{
	DefPlanActif(i);
	ext[i] = LitMini();
	}
e2->AjouteDescripteur(MINSAMPLEVALUE_TIFF,LitNbPlan(),ext);
for (i=0;i<LitNbPlan();i++)
	{
	DefPlanActif(i);
	ext[i] = LitMaxi();
	}
e2->AjouteDescripteur(MAXSAMPLEVALUE_TIFF,LitNbPlan(),ext);

double x= LitResolX();
e2->AjouteDescripteur(XRESOLUTION_TIFF,1,&x);
x =(long)LitResolY();
e2->AjouteDescripteur(YRESOLUTION_TIFF,1,&x);
x =(long)LitResolZ();
e2->AjouteDescripteur(ZRESOLUTION_TIFF,1,&x);

e2->Sauver(*nomFic);
e2->SauverImage((void **)LitPlan(),0);
if (LitNbLigne()>64 && LitNbColonne()>64)
	{
	ImageInfo *im2=ZoomImage(64,64);

	e2->AjouteDescripteur(NEWSUBFILETYPE_TIFF,LONG_TIFF,1,0L,1);
	e2->AjouteDescripteur(IMAGEWIDTH_TIFF,LONG_TIFF,1,im2->LitNbColonne(),1);
	e2->AjouteDescripteur(IMAGELENGTH_TIFF,LONG_TIFF,1,im2->LitNbLigne(),1);
	e2->AjouteDescripteur(COMPRESSION_TIFF,SHORT_TIFF,1,1L,1);
	e2->AjouteDescripteur(PHOTOMETRICINTERPRETATION_TIFF,SHORT_TIFF,1,im2->LitNbPlan(),1);
	e2->AjouteDescripteur(SAMPLESPARPIXEL_TIFF,SHORT_TIFF,0,im2->LitNbPlan(),1L);
	if (im2->LitNbPlan()==1)
		e2->AjouteDescripteur(BITSPERSAMPLE_TIFF,SHORT_TIFF,im2->LitNbPlan(),im2->LitTaillePixel(),1);
	else
		{
		short *p=new short[im2->LitNbPlan()];
		for (long i=0;i<im2->LitNbPlan();i++)
			p[i] =  im2->LitTaillePixel();
		e2->AjouteDescripteur(BITSPERSAMPLE_TIFF,SHORT_TIFF,im2->LitNbPlan(),(char *)p,1);
		delete p;
		}
	e2->AjouteDescripteur(STRIPOFFSET_TIFF,LONG_TIFF,0,0L,1);
	e2->AjouteDescripteur(ROWPERSTRIP_TIFF,LONG_TIFF,1,0L,1);
	e2->AjouteDescripteur(STRIPBYTECOUNT_TIFF,LONG_TIFF,0,0L,1);
	e2->AjouteDescripteur(RESOLUTIONUNIT_TIFF,SHORT_TIFF,1,2L,1);
	e2->AjouteDescripteur(DATETIME_TIFF,ASCII_TIFF,strlen(im2->LitDateCreation())+1,im2->LitDateCreation(),1);

	e2->SauverImage((void **)im2->LitPlan(),1);
	}
delete ext;
}
*/
