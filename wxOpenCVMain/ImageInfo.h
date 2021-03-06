/*#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG
#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#include <stdlib.h>
#include <crtdbg.h>*/
#ifndef __IMAGEINFO__
#define __IMAGEINFO__


#include <ctime>
#include <map>
#include <string>

#include "opencv2/opencv.hpp"
#include "ImageConstante.h"
#include "ParametreOperation.h"
#include "opencv2/core/ocl.hpp"
#include "opencv2/dnn.hpp"



typedef short CodeErreur;
#define ERREUR OuiERREUR

#define NB_OP_MORPHOLOGIE 10
#define NB_OP_CONVOLUTION 40


#define IMAGEINFOCV_ORB_DES 0
#define IMAGEINFOCV_BRISK_DES 1
#define IMAGEINFOCV_AKAZE_DES 2
#define IMAGEINFOCV_BLOB_DES 3
#define IMAGEINFOCV_KAZE_DES 4
#define IMAGEINFOCV_AGAST_DES 5
#define IMAGEINFOCV_GFTT_GRAY_DES 6 // IMAGEINFOCV_GFTT_BLUE_DES
#define IMAGEINFOCV_GFTT_GREEN_DES 7
#define IMAGEINFOCV_GFTT_RED_DES 8
#define IMAGEINFOCV_SIFT_DES 10
#define IMAGEINFOCV_SURF_DES 11
#define IMAGEINFOCV_PRECEDENT_DES 12


class ImageInfoCV : public cv::UMat {
// Type pour les images openCV en binaire type()
// xxxyyy 3 bits de poids faibles pour taille et signe :
// yyy
// 000 non sign� 8 bits, 011 8 bits sign�, 010 non sign� 16 bits, 011 16 bits sign�
// 101 32 bits , 110 64 bits
// xxx bit de poids fort pour le nombre de canaux
// 000 1 canal, 001 2 canaux, 010 3 canaux
public :
    enum {
        SELECT_SOBEL = 0,
        SELECT_SCHARR,
        SELECT_DERICHE,
        SELECT_PAILLOU
    };


static ImageInfoCV **op33;
static ImageInfoCV **op55;
static ImageInfoCV **op77;
static ImageInfoCV **opnn;
static ImageInfoCV **opMorph;

static std::map<std::string, ParametreOperation> listeOperation;

protected :
//

static char *tpFct[9];



void	*eSauver;
private	:
#ifdef __INDOTEXTE__
char	*nomImage;			//< Nom de l'image sur le disque
char	*natureImage;		// M�thode de saisie de l'image
char	dateCreation[30];
char	*createur;			// utilisateur
double	resolX;				// Pixel par cm
double	resolY;				// Pixel par cm
double	resolZ;
char	*uniteZ;
char	*typeMateriel;		// Nom de la chaine d'acquisition
char	*marqueMateriel;	// marque de la chaine d'acquisition
char	*copyright;
char	*nomProjet;			// Nom du projet associ�e � l'image
char	*nomPgm;			// Nom et version du pgm
char	*ordinateur;
char	*description;
char	*champUtil[20];
							//membres utilis�s en fonction du contexte
long	nbChampCopolymere;
char	**copolymere;

long	nbChampGonfle;
char	**gonfle;

long	nbChampComposite;
char	**composite;

long	nbChampMicroscope;
char	**microscope;

long	nbChampCamera;
char	**camera;

long	nbChampScanner;
char	**scanner;

long	nbChampSimulation;
char	**simulation;
#endif

char converCplxEnt;
static int fctImage;
static int typeSeuillage;
static int tailleOperateur;
static int centrageResultat;
static int typeOndelette;
static int tailleOndelette;
static int nbIterOperateur;
static int indOpConvolution; /*< Indice de l'op�rateur de convolution s�lectionn� */
static int indOpMorphologie; /*< Indice de l'op�rateur de morphologie s�lectionn� */
static int typeResultat;	 /*< Type du r�sultat -1, ou constante OpenCV  p256 reference manual */

struct referenceCNN {
    cv::dnn::Net net;
    cv::String modele;
    cv::String proto;
    cv::String labels;
    std::vector<std::string> listeLabels;
};
static std::map<std::string , referenceCNN	 > deep;    /*<! map des r�seaux charg�s */
cv::Mat probCaffe;
cv::Mat objetsYolo;
cv::Mat probYolo;
std::vector<double> probRetenue;
std::vector<std::string> nomClassseCaffe;
std::vector<cv::Rect> rectYolo;
std::vector<std::string> nomClassseYolo;

/***********************************************
********** INFO IMAGE **************************
************************************************/
std::vector<double > minIm;			/*< Minimimum pour chaque plan de l'image */
std::vector<double > maxIm;			/*< Maximimum pour chaque plan de l'image */
std::vector<cv::Point>	locMin;	/*< Position du miminmum pour chaque plan */
std::vector<cv::Point>	locMax;	/*< Position du miminmum pour chaque plan */
std::vector<cv::Mat>		statComposante; /*< Statistique des composantes de chaque plan http://docs.opencv.org/trunk/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html */
std::vector<cv::Mat>		centreGComposante; /*< Centre de gravite http://docs.opencv.org/trunk/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html */
std::vector<std::vector<std::vector<cv::Point> > > contours; /*< Contours dans l'image des composantes connexes http://docs.opencv.org/trunk/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=connectedcomponents#findcontours */
std::vector<std::vector<std::vector<cv::Point> > > contoursPoly; /*< Contours polygonaux dans l'image des composantes connexes http://docs.opencv.org/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html#approxpolydp */
std::vector<std::vector<std::vector<int> > > contoursHull; /*< Contours convexe dans l'image des composantes connexes http://docs.opencv.org/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html#approxpolydp */
std::vector<std::vector<std::vector<cv::Vec4i> > > defautConvexite; /*< D�faut de convexit� http://docs.opencv.org/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=convexitydefect#convexitydefects */
std::vector<std::vector<cv::Vec4i> >  arbreContour; /*< Arborescence des Contours dans l'image des composantes connexes http://docs.opencv.org/trunk/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=connectedcomponents#findcontours */
std::vector<std::vector<cv::Moments> > moment;	/*<!http://docs.opencv.org/doc/tutorials/imgproc/shapedescriptors/moments/moments.html*/
std::vector<std::vector<std::vector<double> > >huMoment;		/*<!http://docs.opencv.org/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=moments#humoments*/
std::vector<std::vector<cv::Moments> > momentCtr;	/*<!http://docs.opencv.org/doc/tutorials/imgproc/shapedescriptors/moments/moments.html*/
std::vector<std::vector<std::vector<double> > >huMomentCtr;		/*<!http://docs.opencv.org/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=moments#humoments*/
std::vector<std::vector<cv::Vec3f> > cercle;		/*<! http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#houghcircles*/
std::vector<std::vector<cv::Vec4i> > ligneP;		/*<! http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#houghlinesp */
std::vector<std::vector<cv::Vec2f> > ligne;		/*<! http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#houghlines */
std::vector<std::vector<cv::Point2f> > boncoin;	/*<! http://docs.opencv.org/modules/imgproc/doc/feature_detection.html#goodfeaturestotrack */
std::vector<std::vector<cv::Point2f> > coinRef;	/*<! Les pixels de r�f�rences de l'image pour calcul du flot optique */
std::map<int,std::vector<cv::KeyPoint> > pointCle;		/*<! map des Points clef avec comme entree IMAGEINFOCV_xxxxxx_DES */
std::map<int,std::vector<cv::KeyPoint> > pointCleApp;		/*<! map des Points clef appari�s (video ou stereo) avec comme entree IMAGEINFOCV_xxxxxx_DES */
std::vector<std::vector <cv::Point > > kMser;	/*<! region et point d�tect�s par mser */
std::map<int,cv::Mat	 > descripteur;		/*<! map des descripteurs avec comme entree IMAGEINFOCV_xxxxxx_DES */
std::map <int,std::vector<cv::DMatch> > matches;	    /*<! map Descripteur appari�s avec entr�e IMAGEINFOCV_xxxxxx_DES */

cv::UMat	*flotOptique;				/*<! Flot optique associ� � l'image calcul� par calcopticalFlowFarnerBack*/
cv::UMat	*ponderation;				/*<! Fenetre de ponderation associ�e � l'image*/
cv::UMat *silh;						/*<! Seuillage de la diff�rence entre deux images pour updateMotion History */

cv::Mat	    masqueMat;			    /*<! Masque pour les op�rations construit � partir des rectangles en attendant de savoir comment lire et �crire dans une UMat */
cv::UMat	masqueOperateur;		/*<! Masque pour les op�rations construit � partir des rectangles */
cv::UMat	*masqueMOG;				/*<! Masque pour calcMotionGradient */
cv::UMat *orient;					/*<! orientation pour calcMotionGradient*/
cv::UMat *segmvt;					/*<! segmentation issue de l'analyse du mouvement (segmentMotion)*/
std::vector<cv::Rect> regionsMvt;	/*<! Regions issues de l'analyse du mouvement (segmentMotion)*/
std::vector<double> angle;			/*<! Angle issus de l'analyse du mouvement (calcGlobalOrientation)*/
cv::Mat homography;                 /*<! Homgraphie calul�e � partir des points appari�s*/

Panoramique         *pano;          /*<! Panoramique construit avec cette image en premier argument  */

std::map<std::string, ParametreOperation> listeOpAttribut;
ParametreOperation *pOCVUpdateMotionHistory;

public :
//	********* Constructeurs et destructeur
ImageInfoCV(long nbL,long nbC,int type);
explicit ImageInfoCV(void);
ImageInfoCV(void *);
ImageInfoCV(char *);
~ImageInfoCV(void);
void DoEnregistrer(char **nomFic,long =0,void * = NULL);
void SauverProprieteProjet(char *);
void DefProprieteImage(char *);
// Lecture des images au format YML
void write(cv::FileStorage& fs) const  ;                      //Write serialization for this class
void read(const cv::FileNode& node);                          //Read serialization for this class


private :
void InitImageInfo(void *eTiff=NULL);
void InitOp();
char **DefChampMultiple(char *,long &);
void DefChampMultiple(long ,char *c,char ***,long &);
char *ConversionChampUtilChampTiff(char **,long,long &);

//	********* D�finition des membres priv�s
public :
void EffaceOp();
void	DefFctImage(int i){fctImage=i;};
void	DefFctImage(char *);
void	DefTypeSeuillage(char*);
void	DefTypeResultat(int i){typeResultat=i;};
void	DefTailleOperateur(char*);
void	DefCentrageResultat(char*);
void	DefTypeOndelette(char *);
void	DefTailleOndelette(long);
void	DefNbIterOperateur(long);
void	DefConverCplxEnt(char c);
int		IndOpConvolution(int i=-1);  /*< Indice de l'op�rateur de convolution s�lectionn� */
int		IndOpMorphologie(int i=-1); /*< Indice de l'op�rateur de morphologie s�lectionn� */
int		FctImage(){return fctImage;};
int		TypeResultat(){return typeResultat;};


void 	DefNomImage(char *);
void 	DefNatureImage(char *);
void 	DefDateCreation(char *);
void 	DefCreateur(char *);
void	DefResolX(double);
void	DefResolY(double);
void	DefResolZ(double z);
void	DefUniteZ(char *);
void	DefChampUtil(char *,long i,long nbOctets=-1);
void	DefChampCopolymere(char *);
void	DefChampCopolymere(long ,char *);
void	DefChampGonfle(char *);
void	DefChampGonfle(long ,char *);
void	DefChampComposite(char *);
void	DefChampComposite(long ,char *);
void	DefChampMicroscope(char *);
void	DefChampMicroscope(long ,char *);
void	DefChampScanner(char *);
void	DefChampScanner(long ,char *);
void	DefChampCamera(char *);
void	DefChampCamera(long ,char *);
void	DefChampSimulation(char *);
void	DefChampSimulation(long ,char *);
void	DefDescription(char *);
void	DefMarqueMateriel(char *);
void	DefTypeMateriel(char *);
void	DefCopyright(char *);
void	DefOrdinateur(char *);
void 	DefNomPgm(char *);
void 	DefNomProjet(char *);

int	LitNbLigne(){return rows;};
int	LitNbColonne(){return cols;};
int	LitNbPlan(){return channels();};

char* 	LitNomImage(void);
char* 	LitNatureImage(void);
char*  	LitDateCreation(void);
char* 	LitCreateur(void);
double	LitResolX(void);
double	LitResolY(void);
double	LitResolZ(void);
char*	LitUniteZ(void);
char*	LitChampUtil(long);
#ifdef __INFOTEXTE__
long	LitNbChampCopolymere(void){return nbChampCopolymere;};
long	LitNbChampComposite(void){return nbChampComposite;};
long	LitNbChampGonfle(void){return nbChampGonfle;};
long	LitNbChampMicroscope(void){return nbChampMicroscope;};
long	LitNbChampScanner(void){return nbChampScanner;};
long	LitNbChampCamera(void){return nbChampCamera;};
long	LitNbChampSimulation(void){return nbChampSimulation;};
#endif
char*	LitChampCopolymere(long );
char*	LitChampComposite(long );
char*	LitChampGonfle(long );
char*	LitChampMicroscope(long );
char*	LitChampScanner(long );
char*	LitChampCamera(long );
char*	LitChampSimulation(long );
char*	LitDescription(void);
char*	LitMarqueMateriel(void);
char*	LitTypeMateriel(void);
char*	LitCopyright(void);
char*	LitOrdinateur(void);
char*	LitNomPgm(void);
char*	LitNomProjet(void);
std::string	InfoImage(void);

//Utilitaire pour info sur l'image
char *InfoPixel(long x,long y,long =-1,long =-1);

// Surcharge des op�rateurs
ImageInfoCV 	*operator* (ImageInfoCV	&z);		// Convolution
ImageInfoCV 	*operator& (ImageInfoCV	&z);		// Erosion
ImageInfoCV 	*operator| (ImageInfoCV	&z);		// Dilatation
ImageInfoCV 	*operator+ (ImageInfoCV	&z);		// Addition
ImageInfoCV 	*operator+ (double	z);
ImageInfoCV 	*operator- (ImageInfoCV	&z);		 // Soustraction
ImageInfoCV 	*operator- (double	z);
ImageInfoCV 	*operator^ (ImageInfoCV	&z);		 // Produit
ImageInfoCV 	*operator^ (double	z);
ImageInfoCV 	*operator/ (ImageInfoCV	&z);		 // Division
ImageInfoCV 	*operator/ (double	z);
ImageInfoCV 	&operator= (ImageInfoCV	&z);
ImageInfoCV	*Variance (long);

// Fonctions membres equivalentes aux op�rateurs utilisant OPENCV
std::vector<ImageInfoCV *>ConvertType(std::vector< ImageInfoCV *>, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV *>EtLogique(std::vector< ImageInfoCV *>, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV *>OuLogique(std::vector< ImageInfoCV *>, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV *>OuExcluLogique(std::vector< ImageInfoCV *>, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV *>Add(std::vector< ImageInfoCV *>, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV *>AddPonderee(std::vector< ImageInfoCV *>, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV *>AbsDiff(std::vector< ImageInfoCV*> op, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV *>Sub(std::vector< ImageInfoCV *>, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV *>Mul(std::vector< ImageInfoCV *>, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV *>Div(std::vector< ImageInfoCV *>, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV *>Erosion(std::vector< ImageInfoCV *>L, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV *>Dilatation(std::vector< ImageInfoCV *>L, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV *>Ouverture(std::vector< ImageInfoCV *>, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV *>Fermeture(std::vector< ImageInfoCV *>, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV *>ChapeauHaut(std::vector< ImageInfoCV *>, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV *>ChapeauBas(std::vector< ImageInfoCV *>, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV *>GradMorph(std::vector< ImageInfoCV *>, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV *>Convolution(std::vector< ImageInfoCV *>, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV	*>PartageEaux(std::vector< ImageInfoCV *>, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV	*>KMeans(std::vector< ImageInfoCV *>, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV*> SuperpixelLSC(std::vector<ImageInfoCV*> op, ParametreOperation * pOCV);
std::vector<ImageInfoCV*> SuperpixelSLIC(std::vector<ImageInfoCV*> op, ParametreOperation * pOCV);
std::vector<ImageInfoCV*> SuperpixelSEEDS(std::vector<ImageInfoCV*> op, ParametreOperation * pOCV);
std::vector<ImageInfoCV	*>GrabCut(std::vector< ImageInfoCV *>, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV	*>CalcBackProject(std::vector< ImageInfoCV *>, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV *>FlotOptiqueLucasKanadePyramide(std::vector< ImageInfoCV *>, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV *>FlotOptiqueFarnerback(std::vector< ImageInfoCV *>, ParametreOperation *pOCV = NULL);
std::vector<ImageInfoCV	*>PhaseCorrelate(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV	*>MAJHistoriqueMvt(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV *>Clahe(std::vector<ImageInfoCV	*> op, ParametreOperation *pOCV);
std::vector<ImageInfoCV *>Inpaint(std::vector<ImageInfoCV	*> op, ParametreOperation *pOCV);

std::vector<ImageInfoCV	*>Fond_MOG(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV	*>Fond_MOG2(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV	*>Fond_KNN(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV	*>Fond_GMG(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);

std::vector<ImageInfoCV		*>ImageInfoCV::LoadDNN(std::vector< ImageInfoCV*> op, ParametreOperation *pOCV);
std::vector<ImageInfoCV		*>ImageInfoCV::ApplyDNN(std::vector< ImageInfoCV*> op, ParametreOperation *pOCV);


std::vector<ImageInfoCV	*>DetailFeaturesFinder(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV	*>DetailMatchesInfo(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV	*>LeaveBiggestComponent(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV	*>HomographyBasedEstimator(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV	*> WraperWrap(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV	*> CorrectionExpo(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV	*> PanoComposition(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);


std::vector<ImageInfoCV *>Negation(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV *>Laplacien(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV *>ScharrX(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV *>ScharrY(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV *>ModuleGradientScharr(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV *>SobelX(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV *>SobelY(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV *>ModuleGradientSobel(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV *>Contour(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV *>ConvexHull(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV *>ConvexityDefects(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV *>ApproxPolyDP(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV *>Seuillage(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV *>SeuillageAdaptatif(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV *>LissageMedian(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV *>LissageMoyenne(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV *>LissageGaussien(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV *>LissageBilateral(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV *>FFT(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV *>IFFT(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>FusionPlan(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>ComposanteConnexe(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>RGB_L(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>DistanceDiscrete(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>Voronoi(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>LigneMediane(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>Canny(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>HoughLigne(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>HoughCercle(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>HoughLigneProba(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>BonAttributs(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>DetectCoinHarris(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>DetectOrb(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>DetectSift(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>DetectSurf(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>DetectBrisk(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>DetectAkaze(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>DetectKaze(std::vector<ImageInfoCV	*> op, ParametreOperation *pOCV);
std::vector<ImageInfoCV	*>DetectAgast(std::vector<ImageInfoCV	*> op, ParametreOperation *pOCV);

std::vector<ImageInfoCV	*>DetectBlob(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>DetectMser(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>AppariePoint(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>FindHomography(std::vector< ImageInfoCV*> op, ParametreOperation *pOCV);

std::vector<ImageInfoCV	*>SeparationPlan(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>CalcOrientationMvt(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>SegmenteMvt(std::vector< ImageInfoCV *>, ParametreOperation *paramOCV);
std::vector<ImageInfoCV	*>TransAffine(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV	*>TransPerspective(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV	*> Dimension(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV	*>CorrigeAberation(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV	*>LogPolar(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV	*>LinearPolar(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);


void ExtremumLoc(ImageInfoCV *mask=NULL );


// Deconvolution it�rative et de wiener
ImageInfoCV 	*Deconvolution (ImageInfoCV	&z);
ImageInfoCV 	*Wiener(ImageInfoCV	&z,double	varianceBruit=0,long larBruit=-1,double seuilFiltre=5);

// Densit� spectrale de puissance
ImageInfoCV 	*DensiteSpectralePuissance (void);
ImageInfoCV 	*DensiteSpectralePuissanceAR (void);

// Transform�e en ondelette
ImageInfoCV 	*DOndelette(void);
ImageInfoCV 	*IOndelette(void);

// Op�rateurs unaires
ImageInfoCV	*Sqr (void);
ImageInfoCV	*Abs (void);

// Seuilage et composantes connexes
long  SeuilImage(float *seuil,int ligDeb=0, int colDeb=0, int hauteur=-1, int largeur=-1);
void  SelectionMaxLocaux(ImageInfoCV &);
ImageInfoCV *Binarisation(float *seuilBin,ImageInfoCV* =NULL);
ImageInfoCV *NonMaximumSuppression(ImageInfoCV* ,ImageInfoCV*,float);
ImageInfoCV *SeuilHysteresis(float seuil1,float seuil2,ImageInfoCV* =NULL);
std::vector<std::vector<cv::Moments> > *CalcMoment();


// Op�rateurs Voronoi
ImageInfoCV 	*Voronoi (ImageInfoCV	&z);

// Squelette
ImageInfoCV	*DistObjetFond();
bool MaxLocal(int c,int i,int j);
bool TraiterMediane(int c,int &r,int &s,int dr,int ds,int &q,cv::Mat &im);
void SuiviChemin(int c,int i,int j,cv::Mat &im);
bool MaxLocal(int i,int j);
bool TraiterMediane(int &r,int &s,int dr,int ds,int &q,cv::Mat &im);
void SuiviChemin(int i,int j,cv::Mat  &im);


// Histogramme
ImageInfoCV *EgalisationHistogramme (void);

// Ajout de bruit � une image
ImageInfoCV *BruitImpulsionnel(double proba=0.25,long iMin=0,long iMax=255);
ImageInfoCV *BruitGaussienMultiplicatif(double variance=3.0);
ImageInfoCV *BruitLaplacien(double v=3.0);

// Op�ration g�om�trique
ImageInfoCV *TransformationGeometrique(double **t, char =0);

// Travaux sp�cifiques � la couleur
void DefDataRGBVersPlan(unsigned char *d);	//Conversion de plan RGB vers Plan R plan G plan B

// Filtrage de deriche et shen
std::vector<ImageInfoCV	*> GradientDericheX(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV	*> GradientDericheY(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
// Filtrage de deriche et shen
std::vector<ImageInfoCV	*> GradientPaillouX(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV	*> GradientPaillouY(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
// Reconstruction � partir de la formule de Shannon
std::vector<ImageInfoCV	*> Recons2d(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV	*> SincXY(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);


// Module du gradient
std::vector<ImageInfoCV	*> ModuleGradient(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);
std::vector<ImageInfoCV	*> ModuleGradientDeriche(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);//double alphaDerive=0.75,double alphaMoyenne=0.25,ImageInfoCV * = NULL,char quadrant=0);
std::vector<ImageInfoCV	*> ModuleGradientPaillou(std::vector< ImageInfoCV *>, ParametreOperation *pOCV);//double alphaDerive=0.75,double alphaMoyenne=0.25,ImageInfoCV * = NULL,char quadrant=0);

// Statistique nanoraptor
ImageInfoCV* ConversionCouleurEnEntier(float );

// Contour
ImageInfoCV *SuiviLigneGradient(ImageInfoCV &,ImageInfoCV &,ImageInfoCV * = NULL);


// Sp�cifique MPI
virtual void DiffusionMPI(void);

// ********* Acc�s aux membres priv�s et
//	Lecture des membres priv�s
void MajMasque(bool actif = false, cv::Rect r = cv::Rect(), int ind=255);
void MajMasque(bool actif, cv::Point r, int ind=255);
std::vector<double > *MinIm(){ if (minIm.size()==0) ExtremumLoc(); return &minIm; };		/*< Minimum de l'image pour chaque canal */
std::vector<double > *MaxIm(){if (maxIm.size()==0) ExtremumLoc();return &maxIm;};		/*< Maximum de l'image pour chaque canal */
char    *LitFctImage(void);
char    *LitTypeSeuillage(void);
char    *LitTailleOperateur(void);
char    *LitCentrageResultat(void);
char	LitValCentrageResultat(void);
char    *LitTypeOndelette(void);
long	LitTailleOndelette(void);
long  	LitNbIterOperateur(void);
char  	LitConverCplxEnt(void);

cv::UMat *MasqueOperateur(){ return &masqueOperateur; };
std::vector<cv::Mat>	*StatComposante(){return &statComposante;};
std::vector<cv::Mat>	*CentreGComposante(){return &centreGComposante;};
cv::UMat	*FlotOptique(bool init=false){if (init ) {delete []flotOptique;flotOptique= new cv::UMat[channels()];} return flotOptique;};
cv::UMat *Ponderation(bool init = false){ if (init) { delete ponderation; ponderation = new cv::UMat();  cv::createHanningWindow(*ponderation, this->size(), CV_64F); } return ponderation; };
cv::UMat *Silh(bool init = false){ if (init) { delete silh; silh = new cv::UMat(); } return silh; };
cv::Mat *Descripteur(char =-1);
std::vector<std::vector<cv::Moments> > *MomentComposante(){ return &moment; };
std::vector<std::vector<cv::Moments> > *MomentCtr(){ return &momentCtr; };

std::vector<std::vector<std::vector<cv::Vec4i> > >*PtDefautConvexite(){return &defautConvexite;};
std::vector<std::vector<std::vector<int> > > *PtContoursHull(){return &contoursHull;};
std::vector<std::vector<std::vector<cv::Point> > > *PtContours(){return &contours;};
std::vector<std::vector<std::vector<cv::Point> > > *PtContoursPoly(){return &contoursPoly;};
std::vector<std::vector<cv::Vec4i> > *ArboContour(){return &arbreContour;}; /*< Arborescence des Contours dans l'image des composantes connexes http://docs.opencv.org/trunk/modules/imgproc/doc/structural_analysis_and_shape_descriptors.html?highlight=connectedcomponents#findcontours */
std::vector<std::vector<std::vector<double> > > *HuMoment(){return &huMoment;};
std::vector<std::vector<std::vector<double> > > *HuMomentCtr(){return &huMomentCtr;};
std::vector<std::vector<cv::Vec3f> > *HoughCercle(){return &cercle;};
std::vector<std::vector<cv::Vec4i> > *HoughLigneProba(){return &ligneP;};
std::vector<std::vector<cv::Vec2f> > *HoughLigne(){return &ligne;};
std::vector<std::vector<cv::Point2f> >*BonCoin(bool init=false){if (init && boncoin.size()==0) boncoin.resize(channels());return &boncoin;};
std::vector<std::vector<cv::Point2f> >*CoinRef(bool init = false){ if (init && coinRef.size() == 0) coinRef.resize(channels()); return &coinRef; };
std::vector<double> *Angle(){ return &angle; };
std::vector<cv::Rect> *RegionMvt(){ return &regionsMvt; };
std::vector<cv::KeyPoint> *PointCle(char type = 0);
std::vector<cv::KeyPoint> *PointCleApp(char type = 0);
std::vector<std::vector<cv::Point> > *PointCleMSER(){return &kMser;};
std::map<int,std::vector<cv::DMatch> > *ListeAppariement(){return &matches;};
std::map<int,std::vector<cv::KeyPoint> > *ListePointCle(){return &pointCle;};
std::map<int,std::vector<cv::KeyPoint> > *ListePointCleApp(){return &pointCleApp;};
std::vector<cv::DMatch> *Appariement(char =-1);
ParametreOperation *ParamOCVUpdateMotionHistory(ParametreOperation *p = NULL);
ParametreOperation *AjoutOpAttribut(ParametreOperation *p);
std::map<std::string,ParametreOperation> *ListeOpAttribut(){return &listeOpAttribut;};
ParametreOperation *OpAttribut(std::string s){if (listeOpAttribut.find(s) != listeOpAttribut.end()) return &listeOpAttribut[s];return NULL;};
Panoramique *ParamPano(){return pano;};

bool CaffeResultPret();
bool YoloResultPret();
int NbRectYolo() { return static_cast<int>(rectYolo.size()); };
cv::Rect RectYolo(int i) { if (i >= 0 && i < rectYolo.size()) return rectYolo[i]; };
double ProbCaffe(int i) { if (i >= 0 && i<probRetenue.size()) return probRetenue[i]; };
double ProbYolo(int i) { if (i >= 0 && i<probRetenue.size()) return probRetenue[i]; };
std::string LabelCaffe(int i) { if (i >= 0 && i < nomClassseCaffe.size()) return nomClassseCaffe[i]; return std::string(); };
std::string LabelYolo(int i) { if (i >= 0 && i < nomClassseYolo.size()) return nomClassseYolo[i]; return std::string(); };


int EtapeOp();  /*<! retourne l'indice de l'�tape de l'op�rateur le plus grand */
void CloneStat(ImageInfoCV *im);
void DeplacerFlotOptique(ImageInfoCV *im);/*<! Fonction d�pla�ant le pointeur flotOptique de im dans this. Celui de im devient nul */
void RazFlotOptique(){flotOptique=NULL;};/*<! R emise � z�ro du pointeur flotOptique */

// MODIFICATION d'une fonction OPENCV
void Threshold( cv::InputArray _src, cv::OutputArray _dst, double thresh, double maxval, int type );

};




#endif //__IMAGEINFO__
