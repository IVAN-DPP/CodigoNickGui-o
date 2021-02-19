/*******************************************/
// Author: Ivan Dario Piernagorda Peña     
// Author: Karen Milena Rodriguez      
// Date:   15/02/2020                  
// Title:  Cuts to Data                              
/*******************************************/


#include "./Histograms.h"
#include "./include/Libraries.h"
#include "./include/Miscelaneous.h"
#include "./src/DataEvent.cpp"

#ifndef CODECUTS_H
#define CODECUTS_H

class Codecuts : public Histograms {

private:

public:
  Codecuts(){DoHistograms();}
  void CodeCuts();

};

void Codecuts::CodeCuts(){

  
  string treeName="g13b";  
  string fileNamePERP="./SKIMS/ListFiles.txt";
  DataEvent *myDataList=new DataEvent(fileNamePERP,treeName, 35);
  
  vector<string> PolTableName;
  ListFilesAtDir("./TABLES/ListTables", PolTableName);
  
  map<vector<float>,int> keysPlane;

  //Average Table Polarization
  vector<double> vecInitDob(2);   vector<vector<double>> AvP(10,vecInitDob);
  vector<int> vecInitInt(2);      vector<vector<int>> ItP(10,vecInitInt);

  //Table Events X 14 cuts
  vector<int> Events(19);
  
  const int NumbOfPolFiles=PolTableName.size();
  for (int i=0;i<NumbOfPolFiles;i++){
    char *cstr = const_cast<char *>(PolTableName[i].c_str());
    LoadPolTable(i,cstr,keysPlane);
  }
  
  while (myDataList->getEntry()<myDataList->getEntries()){

    myDataList->getNextEntry();
    if (myDataList->getEntry() % 1000 == 0){
      if(myDataList->getCoh_plan() == 0){
	fprintf (stderr, "Looped %s : %.2f percent, CohEdge %f BeamE %f \r", "PARA" ,myDataList->getEntry()*100.0/myDataList->getEntries(),myDataList->getCoh_edge_nom(),myDataList->getBeam_en());
	fflush (stderr);
      }
      else {
	fprintf (stderr, "Looped %s : %.2f percent, CohEdge %f BeamE %f \r", "PERP" ,myDataList->getEntry()*100.0/myDataList->getEntries(),myDataList->getCoh_edge_nom(),myDataList->getBeam_en());
	fflush (stderr);
      }
    }
      
      
    Events[0]++;         //Events Without Cuts

    //------------------ Delta Beta ---------------//
    double deltbeta[3]     = {};
    double deltbetacut[3]  = {};
      
    //---------- Vertex ------------ //
      
      
    h_Vertex->Fill(myDataList->getEVNT_vertex(1).Z());
    if(myDataList->getEVNT_vertex(1).Z()<-39.0  || myDataList->getEVNT_vertex(1).Z()>-1.0) continue;

    Events[1]++;         //Events With Vertex Cut
    
    for (int i=0;i<myDataList->getNum_chargedtracks();i++){
      deltbeta[i]=myDataList->getEVNT_track(i).Beta()-myDataList->getEVNT_bem(i);
      h_DeltaBe[i]->Fill(myDataList->getEVNT_track(i).Rho(),deltbeta[i]);
      h_BeVSp[i]->Fill(myDataList->getEVNT_track(i).Rho(),myDataList->getEVNT_bem(i));
      h_BeVSpT->Fill(myDataList->getEVNT_track(i).Rho(),myDataList->getEVNT_bem(i));

      if(i == 0 || i == 1)
	h_Mass[0]->Fill(myDataList->getEVNT_track(i).Mt());
      else
	h_Mass[1]->Fill(myDataList->getEVNT_track(i).Mt());
    }

    
    
    //------------------ Delta Beta with Cuts ---------------//
    
    for (int i=0;i<myDataList->getNum_chargedtracks();i++){
      deltbetacut[i]=myDataList->getEVNT_track(i).Beta()-myDataList->getEVNT_bem(i);
	
      if(deltbetacut[2] > 0.05  || deltbetacut[2] < -0.05) continue; 
      if(deltbetacut[0] > 0.02  || deltbetacut[0] < -0.02) continue;
      if(deltbetacut[1] > 0.025 || deltbetacut[1] < -0.025) continue;


      h_DeltaBecut[i]->Fill(myDataList->getEVNT_track(i).Rho(),deltbeta[i]);
      h_BeVSpcut[i]->Fill(myDataList->getEVNT_track(i).Rho(),myDataList->getEVNT_bem(i));

    }

    if(deltbetacut[2] > 0.05  || deltbetacut[2] < -0.05) continue;
    Events[2]++;         //Events With DeltaB cut
    if(deltbetacut[0] > 0.02  || deltbetacut[0] < -0.02) continue;
    Events[3]++;         //Events With DeltaB cut
    if(deltbetacut[1] > 0.025 || deltbetacut[1] < -0.025) continue;
    Events[4]++;         //Events With DeltaB cut
       
    //------------------Correlation Theta-Phi, -----------------------/
    
    h_ThePhi[0]->Fill(myDataList->getEVNT_track(0).Phi()*TMath::RadToDeg(), myDataList->getEVNT_track(0).Theta()*TMath::RadToDeg());  
    h_ThePhi[1]->Fill(myDataList->getEVNT_track(1).Phi()*TMath::RadToDeg(), myDataList->getEVNT_track(1).Theta()*TMath::RadToDeg());
    h_ThePhi[2]->Fill(myDataList->getEVNT_track(2).Phi()*TMath::RadToDeg(), myDataList->getEVNT_track(2).Theta()*TMath::RadToDeg());
    
    
    //-----Cuts due to detector geometry (Fiduciary cuts)---------//
    //------------------------------------------------------------//
      
    Double_t phiproton_cut;
    phiproton_cut= myDataList->getEVNT_track(0).Phi()*TMath::RadToDeg();
    
    if( (phiproton_cut <= -25)   && (phiproton_cut >= -35)  ) continue;
    if( (phiproton_cut <= -85)   && (phiproton_cut >= -95)  ) continue;
    if( (phiproton_cut <= -145)  && (phiproton_cut >= -155) ) continue;
    if( (phiproton_cut >= 25)    && (phiproton_cut <= 35)   ) continue;
    if( (phiproton_cut >= 85)    && (phiproton_cut <= 95)   ) continue;
    if( (phiproton_cut >= 145)   && (phiproton_cut <= 155)  ) continue;

    if(F_ThePhiProt[0]->Eval(phiproton_cut) > myDataList->getEVNT_track(0).Theta()*TMath::RadToDeg() ||
       F_ThePhiProt[1]->Eval(phiproton_cut) > myDataList->getEVNT_track(0).Theta()*TMath::RadToDeg() ||
       F_ThePhiProt[2]->Eval(phiproton_cut) > myDataList->getEVNT_track(0).Theta()*TMath::RadToDeg() ||
       F_ThePhiProt[3]->Eval(phiproton_cut) > myDataList->getEVNT_track(0).Theta()*TMath::RadToDeg() ||
       F_ThePhiProt[4]->Eval(phiproton_cut) > myDataList->getEVNT_track(0).Theta()*TMath::RadToDeg() ||
       F_ThePhiProt[5]->Eval(phiproton_cut) > myDataList->getEVNT_track(0).Theta()*TMath::RadToDeg() ||
       F_ThePhiProt[6]->Eval(phiproton_cut) > myDataList->getEVNT_track(0).Theta()*TMath::RadToDeg()) continue;
       
    h_ThePhicut[0]->Fill(phiproton_cut, myDataList->getEVNT_track(0).Theta()*TMath::RadToDeg());

    Events[5]++;         //Events With Phi-Theta Cuts
    //kaon cuts
    Double_t phikaon_cut;
    phikaon_cut= myDataList->getEVNT_track(1).Phi()*TMath::RadToDeg();
        
    if( (phikaon_cut <= -25)   && (phikaon_cut >= -35)  ) continue;
    if( (phikaon_cut <= -85)   && (phikaon_cut >= -95)  ) continue;
    if( (phikaon_cut <= -145)  && (phikaon_cut >= -155) ) continue;
    if( (phikaon_cut >= 25)    && (phikaon_cut <= 35)   ) continue;
    if( (phikaon_cut >= 85)    && (phikaon_cut <= 95)   ) continue;
    if( (phikaon_cut >= 145)   && (phikaon_cut <= 155)  ) continue;

    if(F_ThePhiProt[0]->Eval(phikaon_cut) > myDataList->getEVNT_track(1).Theta()*TMath::RadToDeg() ||
       F_ThePhiProt[1]->Eval(phikaon_cut) > myDataList->getEVNT_track(1).Theta()*TMath::RadToDeg() ||
       F_ThePhiProt[2]->Eval(phikaon_cut) > myDataList->getEVNT_track(1).Theta()*TMath::RadToDeg() ||
       F_ThePhiProt[3]->Eval(phikaon_cut) > myDataList->getEVNT_track(1).Theta()*TMath::RadToDeg() ||
       F_ThePhiProt[4]->Eval(phikaon_cut) > myDataList->getEVNT_track(1).Theta()*TMath::RadToDeg() ||
       F_ThePhiProt[5]->Eval(phikaon_cut) > myDataList->getEVNT_track(1).Theta()*TMath::RadToDeg() ||
       F_ThePhiProt[6]->Eval(phikaon_cut) > myDataList->getEVNT_track(1).Theta()*TMath::RadToDeg()) continue;

    h_ThePhicut[1]->Fill(phikaon_cut, myDataList->getEVNT_track(1).Theta()*TMath::RadToDeg());

    Events[6]++;         //Events With Phi-Theta Cuts   
      
    //Pion cuts
    
    Double_t phiPion_cut;
    phiPion_cut= myDataList->getEVNT_track(2).Phi()*TMath::RadToDeg();
      
    if( (phiPion_cut <= -25)   && (phiPion_cut >= -35)  ) continue;
    if( (phiPion_cut <= -85)   && (phiPion_cut >= -95)  ) continue;
    if( (phiPion_cut <= -145)  && (phiPion_cut >= -155) ) continue;
    if( (phiPion_cut >= 25)    && (phiPion_cut <= 35)   ) continue;
    if( (phiPion_cut >= 85)    && (phiPion_cut <= 95)   ) continue;
    if( (phiPion_cut >= 145)   && (phiPion_cut <= 155)  ) continue;

    if(F_ThePhiProt[0]->Eval(phiPion_cut) > myDataList->getEVNT_track(2).Theta()*TMath::RadToDeg() ||
       F_ThePhiProt[1]->Eval(phiPion_cut) > myDataList->getEVNT_track(2).Theta()*TMath::RadToDeg() ||
       F_ThePhiProt[2]->Eval(phiPion_cut) > myDataList->getEVNT_track(2).Theta()*TMath::RadToDeg() ||
       F_ThePhiProt[3]->Eval(phiPion_cut) > myDataList->getEVNT_track(2).Theta()*TMath::RadToDeg() ||
       F_ThePhiProt[4]->Eval(phiPion_cut) > myDataList->getEVNT_track(2).Theta()*TMath::RadToDeg() ||
       F_ThePhiProt[5]->Eval(phiPion_cut) > myDataList->getEVNT_track(2).Theta()*TMath::RadToDeg() ||
       F_ThePhiProt[6]->Eval(phiPion_cut) > myDataList->getEVNT_track(2).Theta()*TMath::RadToDeg()) continue;

    
    h_ThePhicut[2]->Fill(phiPion_cut, myDataList->getEVNT_track(2).Theta()*TMath::RadToDeg());

    Events[7]++;         //Events With Phi-Theta Cuts   
    
    //------------------ Photons, Delta T  ------------------ // 
      
    for (int i=0;i<myDataList->getNum_photons();i++){
      //LOOP OVER ALL PHOTONS
      if (fabs(deltbeta[1])<0.025){
	h_DeltaTall[0]->Fill(myDataList->getDelt_t_k(i));
	h_DeltaTallvsp[0]->Fill(myDataList->getEVNT_track(1).Rho(),myDataList->getDelt_t_k(i));
      }
      if (fabs(deltbeta[2])<0.05){
	h_DeltaTall[1]->Fill(myDataList->getDelt_t_pi(i));
	h_DeltaTallvsp[1]->Fill(myDataList->getEVNT_track(2).Rho(),myDataList->getDelt_t_pi(i));
      }
    }
      
    //------------Delta T with Cuts ----------- //
      
    if (myDataList->getNumph_k()==1)
      h_DeltaT[0]->Fill(myDataList->getDelt_t_k(myDataList->getIndex_k(0)));
    if(myDataList->getNumph_pi()==1)
      h_DeltaT[1]->Fill(myDataList->getDelt_t_pi(myDataList->getIndex_pi(0)));
      
    if (myDataList->getNumph_k()!=1) continue;
    Events[8]++;         //Events With Delta T Photon cut
    //if (myDataList->getNumph_pi()!=1) continue;
      
    //--------------- Energy loss ----------- //
    double CorrElossKa, CorrElossPr, CorrElossPi;

    CorrElossKa=(myDataList->geteloss_track(0).Rho() - myDataList->getEVNT_track(0).Rho() )/myDataList->getEVNT_track(0).Rho();
    CorrElossPr=(myDataList->geteloss_track(1).Rho() - myDataList->getEVNT_track(1).Rho() )/myDataList->getEVNT_track(1).Rho();
    CorrElossPi=(myDataList->geteloss_track(2).Rho() - myDataList->getEVNT_track(2).Rho() )/myDataList->getEVNT_track(2).Rho();
      
    h_eloss[0]->Fill(CorrElossKa);
    h_eloss[1]->Fill(CorrElossPr);
    h_eloss[2]->Fill(CorrElossPi);

    h_Celoss[0]->Fill(myDataList->getEVNT_track(0).Rho(), CorrElossKa);
    h_Celoss[1]->Fill(myDataList->getEVNT_track(1).Rho(), CorrElossPr);
    h_Celoss[2]->Fill(myDataList->getEVNT_track(2).Rho(), CorrElossPi);
    
    


    

    //--------------- Coh Edge -------------- //
    
      
      
    h_TagrEpho[0]->Fill(myDataList->getTAGR_epho(myDataList->getIndex_k(0))*1000.0);
    if (myDataList->getTAGR_epho(myDataList->getIndex_k(0))*1000.0 > myDataList->getCoh_edge()) continue;
    h_TagrEpho[1]->Fill(myDataList->getTAGR_epho(myDataList->getIndex_k(0))*1000.0);        Events[9]++;         //Events With Tager Epho
    if (myDataList->getTAGR_epho(myDataList->getIndex_k(0))*1000< myDataList->getCoh_edge()-200.0) continue;
    h_TagrEpho[2]->Fill(myDataList->getTAGR_epho(myDataList->getIndex_k(0))*1000.0);        Events[10]++;         //Events With Tager Epho
    if (fabs(myDataList->getCoh_edge()-myDataList->getCoh_edge_nom()*1000)>15)continue;     Events[11]++;         //Events With Tager Epho
    if (myDataList->getTrip_flag()!=0)continue;                                             Events[12]++;         //Events With Tager Epho
    if (myDataList->getCoh_plan()!=0 && myDataList->getCoh_plan()!=1)continue;              Events[13]++;         //Events With Tager Epho

    vector<float> Keys(3);
    if(myDataList->getCoh_edge_nom() == float(1.3)){
      Keys[0]=float(4.2);
      Keys[1]=myDataList->getCoh_edge_nom();
      Keys[2]=float(myDataList->getCoh_plan());
  }
    else{
      Keys[0]=float(myDataList->getBeam_en());
      Keys[1]=myDataList->getCoh_edge_nom();
      Keys[2]=float(myDataList->getCoh_plan());
  }
        
    double PhotoPol=0;
    PhotoPol=GetPol(keysPlane[Keys], myDataList->getCoh_edge(), myDataList->getTAGR_epho(myDataList->getIndex_k(0))*1000.0, 8, 0.2,0.3);
    
    if (PhotoPol<0.5) continue;
    
    GetPolAv(Keys,ItP,AvP,PhotoPol);
    Events[14]++;         //Events With PhotoPol Tables

    //-------------- Reconstruction --------- //
      
    TLorentzVector photon, deuteron, kaon, proton ,pion, Neutron, WBoost;			//Principal Reaction
    TLorentzVector kaonpion, kaonproton, pionkaon;						//MIS-identification particles
    TLorentzVector MMNeut_kaon, MMNeut_KPi, MMNeut_KP, MMNeut_PiK ,MMSigma;			//Missing mass
    TLorentzVector Sigma, Lambda;								//Invariant Mass
    
    photon.SetXYZM(0,0,myDataList->getTAGR_epho(myDataList->getIndex_k(0)),0);
    deuteron.SetXYZM(0,0,0,1.8756);
    
    double Px_kaonpion = myDataList->getEVNT_track(1).Rho()* sin(myDataList->getEVNT_track(1).Theta())* cos(myDataList->getEVNT_track(1).Phi());
    double Py_kaonpion = myDataList->getEVNT_track(1).Rho()* sin(myDataList->getEVNT_track(1).Theta())* sin(myDataList->getEVNT_track(1).Phi());
    double Pz_kaonpion = myDataList->getEVNT_track(1).Rho()*(myDataList->getEVNT_track(1).CosTheta());

    double Px_pionkaon = myDataList->getEVNT_track(2).Rho()* sin(myDataList->getEVNT_track(2).Theta())* cos(myDataList->getEVNT_track(2).Phi());
    double Py_pionkaon = myDataList->getEVNT_track(2).Rho()* sin(myDataList->getEVNT_track(2).Theta())* sin(myDataList->getEVNT_track(2).Phi());
    double Pz_pionkaon = myDataList->getEVNT_track(2).Rho()*(myDataList->getEVNT_track(2).CosTheta());

    // This mass is of Pion-, because we need remove the background of Pion-
    kaonpion.SetXYZM(Px_kaonpion, Py_kaonpion, Pz_kaonpion, 0.139);       			
    kaonproton.SetXYZM(Px_kaonpion, Py_kaonpion, Pz_kaonpion,0.9383);
    pionkaon.SetXYZM(Px_pionkaon, Py_pionkaon, Pz_pionkaon,0.4937);
    
    proton 	= myDataList->geteloss_track(0);
    kaon 	= myDataList->geteloss_track(1);
    pion 	= myDataList->geteloss_track(2);
    MMNeut_kaon = photon + deuteron - proton - kaon - pion;
    Neutron.SetXYZM(MMNeut_kaon.Px(), MMNeut_kaon.Py(), MMNeut_kaon.Pz(), 0.939);
    Sigma 	= pion + Neutron;
    Lambda 	= pion + proton;
    MMSigma  	= photon + deuteron - proton - kaon;           			// Correlation with invariant mass (lambda)
    WBoost   	= photon + deuteron; 						// to make Boost
      
    MMNeut_KPi 	= photon + deuteron - proton - kaonpion - pion;     	   	// This missing mass is with the Pion-
    MMNeut_KP	= photon + deuteron - proton - kaonproton - pion;     	   	// This missing mass is with the Proton
    MMNeut_PiK 	= photon + deuteron - proton - pionkaon - kaon;     	   	// This missing mass is with the Kaon-      
      
    h_MissingMass->Fill(MMNeut_kaon.M());

    h_MissingMass_kaonpion->Fill(MMNeut_KPi.M());
    h_MissingMass_kaonproton->Fill(MMNeut_KP.M());
    h_MissingMass_pionkaon->Fill(MMNeut_PiK.M());
    //h_MissingPvsIMMass->Fill(MMNeut_kaon.M(),MMNeut_kaon.P());
    h_MissingMass_vsMissingMasskaonpion[0]->Fill(MMNeut_kaon.M(), MMNeut_KPi.M());
    h_MissingMass_vsMissingMasskaonproton[0]->Fill(MMNeut_kaon.M(),MMNeut_KP.M());
    h_MissingMass_vsMissingMasspionkaon[0]->Fill(MMNeut_kaon.M(),MMNeut_PiK.M());
    
            
    
    if(MMNeut_KPi.M() < 0.98) continue;       //Cut from correlation MM
    h_MissingMass_vsMissingMasskaonpion[1]->Fill(MMNeut_kaon.M(), MMNeut_KPi.M());
    if(MMNeut_KP.M()  > 0.75) continue; 
    h_MissingMass_vsMissingMasskaonproton[1]->Fill(MMNeut_kaon.M(),MMNeut_KP.M());
    if(MMNeut_PiK.M() > 0.70) continue; 
    h_MissingMass_vsMissingMasspionkaon[1]->Fill(MMNeut_kaon.M(),MMNeut_PiK.M());
    Events[15]++;         //Events With NOT PION, YES Kaon
    // Double_t El = TMath::Power((proton.P()-offsetx)*cos(angle)+(MMSigma.M()-offsety)*sin(angle),2)/TMath::Power(radx,2)
    //   +TMath::Power((proton.P()-offsetx)*sin(angle)-(MMSigma.M()-offsety)*cos(angle),2)/TMath::Power(rady,2);
    // if(El <= 1) continue;
    



    
    //----------Correlación momentums vs missing mass------------------------//
      
    if( Lambda.M()<1.11 || Lambda.M()>1.132) 
      h_MissingPvsIMMass[0]->Fill(Sigma.M(),MMNeut_kaon.P());
    
    if( Sigma.M()<1.08 || Sigma.M()>1.3)
      h_MissingPvsIMMass[1]->Fill(Lambda.M(),MMNeut_kaon.P());

     h_MissingMassvsSigmaMass->Fill(Sigma.M(), MMNeut_kaon.P());
    
    h_MissingMasscut->Fill(MMNeut_kaon.M());
    h_MissingMass_kaonpioncut->Fill(MMNeut_KPi.M());
      
    // h_MissingP->Fill(MMNeut_kaon.P());
      
    h_MissingPvsSigmaMass->Fill(Sigma.M(), MMNeut_kaon.P());
          
    h_InvariantMass->Fill(Sigma.M());
    h_LambdaMass->Fill(Lambda.M());
    h_InvMassLambda_vsInvMassSigma->Fill(Sigma.M(), Lambda.M());
    h_MMassSigma->Fill(MMSigma.M());
    h_MMNeutron_vsMMassSigma[0]->Fill(MMNeut_kaon.M(),MMSigma.M());

    
    //------------------------Comparación de sigmas-------------//
    if( Lambda.M()<1.108 || Lambda.M()>1.124)
      h_InvariantMasscut[0]->Fill(Sigma.M());
      
    if( Lambda.M()<1.11 || Lambda.M()>1.132)
      h_InvariantMasscut[1]->Fill(Sigma.M());
      
    if( Lambda.M()<1.092 || Lambda.M()>1.14)
      h_InvariantMasscut[2]->Fill(Sigma.M());
      
      
    //------------- Comparación de missing momentums-----------//
      
    if( Lambda.M()<1.096 || Lambda.M()>1.136) 
      h_MissingP[0]->Fill(MMNeut_kaon.P());   
    if( Sigma.M()<1.08 || Sigma.M()>1.3)
      h_MissingP[1]->Fill(MMNeut_kaon.P());


    if ( Lambda.M()>=1.1 && Lambda.M()<=1.132) continue;                   //Cut for LamdaMass in +/- 8sigma
    Events[16]++;         //Events With Lambda cuts


    //--------Correlation Momentums--------------//
    h_CorrelationMMomentum->Fill(Sigma.P(), Lambda.P());
    
    // Events[17]++;         //Events With Mass neutron range
    if(MMNeut_kaon.P()<=0.2) continue;                                    //Cut for rescattering
    Events[18]++;         //Events With Neutron Rescattering Lambada cuts


    h_MMNeutron_vsMMassSigma[1]->Fill(MMNeut_kaon.M(),MMSigma.M());       //Cut MM neutron and MM sigma 
    h_InvariantMasscut[3]->Fill(Sigma.M());
   
    h_MMassSigmaCut->Fill(MMSigma.M());
      
    // Pion -  (Por si las moscas)
    h_DeltaBVSInvariantMass->Fill(Sigma.M(),deltbeta[2]);
    h_DeltaBVSMissingMass->Fill(MMNeut_kaon.M(),deltbeta[2]);
    h_DeltaBVSMissingMomentum->Fill(MMNeut_kaon.P(),deltbeta[2]);

    //------------Momentum proton----------------.//
    h_MomentumProton->Fill(proton.P());
   
    //-----------------BOOST------------------------------//

    TVector3 b=WBoost.BoostVector();
    kaon.Boost(-b);
    double KaonCosThetaCM=TMath::Cos(kaon.Theta());
    double KaonPhiCM=kaon.Phi()*TMath::RadToDeg();
    h_KCosThetaCM->Fill(KaonCosThetaCM);

    //---------------Bins Cos Theta Kaon----------------//

    //0 is for PARA
    //1 is for PERP
    if (myDataList->getCoh_plan()==0){
      if(KaonCosThetaCM < 0.51){
	MEASPhi.at(0).push_back(KaonPhiCM);
	MEASGammaP.at(0).push_back(PhotoPol);
	h_kaonPhiPA[0]->Fill(KaonPhiCM);
      }
      else if (KaonCosThetaCM > 0.51){
	MEASPhi.at(1).push_back(KaonPhiCM);
	MEASGammaP.at(1).push_back(PhotoPol);
	h_kaonPhiPA[1]->Fill(KaonPhiCM);
      }
    }
      
    else if (myDataList->getCoh_plan()==1){
      if(KaonCosThetaCM < 0.51){
	MEASPhi.at(0).push_back(KaonPhiCM);
	MEASGammaP.at(0).push_back(-PhotoPol);
	h_kaonPhiPE[0]->Fill(KaonPhiCM);
      }
      else if (KaonCosThetaCM > 0.51){
	MEASPhi.at(1).push_back(KaonPhiCM);
	MEASGammaP.at(1).push_back(-PhotoPol);
	h_kaonPhiPE[1]->Fill(KaonPhiCM);
      }
    }


    //---------------Asymmetry Analysis----------------//
      
    h_Asym[0]=(TH1F*)h_kaonPhiPA[0]->GetAsymmetry(h_kaonPhiPE[0]);
    h_Asym[1]=(TH1F*)h_kaonPhiPA[1]->GetAsymmetry(h_kaonPhiPE[1]);
      
      

      
  }
  cout<<endl;

  GetPolAvTable(ItP,AvP);
  GetPolAvTableLatex(ItP, AvP, "./PolTable.tex","Polarization Tables","poltab");
  GetEventPercentLatex(Events, "./EventCuts.tex", "Event Cut Tables", "eventtab");
  GetEventPercent(Events);
  DoCanvas();
  
}

#endif
