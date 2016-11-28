CASEID  TGX
 $FLTCON 
         NMACH=1.,
         MACH=.41,
         NALPHA=7.,
         ALPHA=0.,-0.5,-1.,-1.5,-2.0,-2.5,-3.0
         BETA=0.0,
         PHI=0.0,
         ALT=0.0,
 $END
 $REFQ 
         XCG=41.18,
         LREF=9.0,
         SREF=63.61725,
         BLAYER=NATURAL, 
         RHR=1300.0,
         SCALE=1.0,
 $END
 $AXIBOD 
         XO=0.000,
         TNOSE=OGIVE, 
         LNOSE=12.68,
         DNOSE=9.0, 
         BNOSE=3.5,
         TRUNC=.FALSE.,
         LCENTR=63.0, 
         DCENTR=9.0,
         TAFT=OGIVE,
         LAFT=11.5,
         DAFT=6.0,
         DEXIT=0.,
 $END
 $FINSET1
         SECTYP=NACA,
         SSPAN= 4.5, 11.93,
         CHORD= 20.38, 0.000,
         XLE=  55.3,
         SWEEP= 70.0,90.0,
         STA=0.,0.,
         NPANEL=6.0,
         PHIF= 30.,90.,150.,210.,270.,330.,
         GAM= 0.0,0.0,0.0,0.0,0.0,0.0,
 $END
PLOT
DERIV DEG
NO LAT
NACA 1 4 0002
DIM IN
BUILD
NEXT CASE
