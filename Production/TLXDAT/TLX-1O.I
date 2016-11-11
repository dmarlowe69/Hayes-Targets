CASEID  TLX-1 Ogive nose with Canards
 $FLTCON 
         NMACH =1., MACH =.41,
         NALPHA=6., ALPHA=0.,1.,2.,3.,4.,5.,
         PHI=0.0,
         ALT=0.0,
 $END
 $REFQ 
         XCG=58.0,
         LREF=9.0,
         SREF=63.61725,
         BLAYER=NATURAL, 
         RHR=50.0,
         SCALE=1.0,
 $END
 $AXIBOD 
         XO=0.000,
         TNOSE=OGIVE, 
         LNOSE=18.00,
         DNOSE=9.0, 
         TRUNC=.FALSE.,
         LCENTR=82.0, 
         DCENTR=9.0,
         TAFT=OGIVE,
         LAFT=11.5,
         DAFT=6.0,
         DEXIT=0.,
 $END
 $FINSET1
         SECTYP=NACA,
         SSPAN= 4.5, 12.0,
         CHORD= 4.0, 4.0,
         XLE=  57.0, 57.0,
         NPANEL=2.0,
         PHIF= 90.,270.,
 $END
 $FINSET2
         SECTYP=NACA,
         SSPAN= 4.5, 11.93,
         CHORD= 20.38, 0.000,
         XLE=  83.12,
         SWEEP= 70.0,90.0,
         STA=0.,0.,
         NPANEL=6.0,
         PHIF= 30.,90.,150.,210.,270.,330.,
         GAM= 0.0,0.0,0.0,0.0,0.0,0.0,
 $END
DERIV DEG
NO LAT
PART
NACA 1 4 0018
NACA 2 4 0001
DIM IN
BUILD
NEXT CASE

