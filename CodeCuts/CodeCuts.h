#include "Histograms.h"
#include "Libraries.h"
//#include "Miscelaneous.h"


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
  string fileNamePERP="../List1.txt";
  DataEvent *myDataPERP=new DataEvent(fileNamePERP,treeName, 35);

  /*
  vector<string> filename;
  vector<string> PolTableName;
  
  PolTableName.push_back("./Tables/Beam_3309_CohEdge_1.3_AUTO_and_AMO_PARA_1273.0.fitted2");//1
  PolTableName.push_back("./Tables/Beam_3309_CohEdge_1.3_AUTO_and_AMO_PARA_1274.0.fitted2");//2
  
  PolTableName.push_back("./PolTables_ce13_42_PERP.dat");//3
  PolTableName.push_back("./PolTables_ce15_41_PARA.dat");//4
  PolTableName.push_back("./PolTables_ce15_41_PERP.dat");//5
  PolTableName.push_back("./PolTables_ce15_45_PARA.dat");//6
  PolTableName.push_back("./PolTables_ce15_45_PERP.dat");//7
  PolTableName.push_back("./PolTables_ce17_41_PARA.dat");//8
  PolTableName.push_back("./PolTables_ce17_41_PERP.dat");//9
  PolTableName.push_back("./PolTables_ce17_47_PARA.dat");//10
  PolTableName.push_back("./PolTables_ce17_47_PERP.dat");//11
  //PolTableName.push_back("./PolTables_ce19_51_PARA.dat");//12
  //PolTableName.push_back("./PolTables_ce19_51_PERP.dat");//13
  PolTableName.push_back("./PolTables_ce19_51_PERPnewTable.dat");//13
  PolTableName.push_back("./PolTables_ce21_51_PARA.dat");//14
  PolTableName.push_back("./PolTables_ce21_51_PERP.dat");//15
  PolTableName.push_back("./PolTables_ce21_52_PARA.dat");//16
  PolTableName.push_back("./PolTables_ce21_52_PERP.dat");//17
  PolTableName.push_back("./PolTables_ce23_52_PARA.dat");//18
  PolTableName.push_back("./PolTables_ce23_52_PERP.dat");//19
  PolTableName.push_back("./PolTables_ce13_33_PARA.dat");//20
  PolTableName.push_back("./PolTables_ce13_33_PERP.dat");//21
  */
  
  /*
  ListFilesAtDir("./PARA","root",filename);
  ListFilesAtDir("./PERP","root",filename);
  
  const int NumbOfFiles=filename.size();

  DataEvent *myData[NumbOfFiles];
  for (int i=0;i<NumbOfFiles;i++){
    myData[i]=new DataEvent(filename.at(i),treeName);
  }

  //ListFilesAtDir("./Tables", PolTableName);
  
  const int NumbOfPolFiles=PolTableName.size();
  for (int i=0;i<NumbOfPolFiles;i++){
    char *cstr = const_cast<char *>(PolTableName[i].c_str());
    cout << cstr << endl;
    LoadPolTable(i,cstr);
  }

  //exit(1);
  */

  while (myDataPERP->getEntry()<myDataPERP->getEntries()){
    myDataPERP->getNextEntry();
    if (myDataPERP->getEntry() % 1000 == 0){
      fprintf (stderr, "Looped over PERP %.2f percent \r", myDataPERP->getEntry()*100.0/myDataPERP->getEntries());
      fflush (stderr);
    }

    //------------------ Delta Beta ---------------//
    double deltbeta[3]     = {};
    double deltbetacut[3]  = {};

    //---------- Vertex ------------ //

    
    h_Vertex->Fill(myDataPERP->getEVNT_vertex(1).Z());
    if(myDataPERP->getEVNT_vertex(1).Z()<-39.0  || myDataPERP->getEVNT_vertex(1).Z()>-1.0) continue;
    
    for (int i=0;i<myDataPERP->getNum_chargedtracks();i++){
      deltbeta[i]=myDataPERP->getEVNT_track(i).Beta()-myDataPERP->getEVNT_bem(i);
      h_DeltaBe[i]->Fill(myDataPERP->getEVNT_track(i).Rho(),deltbeta[i]);
      h_BeVSp[i]->Fill(myDataPERP->getEVNT_track(i).Rho(),myDataPERP->getEVNT_bem(i));
      h_BeVSpT->Fill(myDataPERP->getEVNT_track(i).Rho(),myDataPERP->getEVNT_bem(i));
    }

    //------------------ Delta Beta with Cuts ---------------//
    
    for (int i=0;i<myDataPERP->getNum_chargedtracks();i++){
      deltbetacut[i]=myDataPERP->getEVNT_track(i).Beta()-myDataPERP->getEVNT_bem(i);

      
      if(deltbetacut[0] > 0.05  || deltbetacut[0] < -0.045) continue;
      if(deltbetacut[1] > 0.025 || deltbetacut[1] < -0.025) continue;
      if(deltbetacut[2] > 0.05  || deltbetacut[2] < -0.05) continue;
      h_DeltaBecut[i]->Fill(myDataPERP->getEVNT_track(i).Rho(),deltbeta[i]);
      h_BeVSpcut[i]->Fill(myDataPERP->getEVNT_track(i).Rho(),myDataPERP->getEVNT_bem(i));
    }
    
    if(deltbetacut[0] > 0.05  || deltbetacut[0] < -0.045) continue;
    if(deltbetacut[1] > 0.025 || deltbetacut[1] < -0.025) continue;
    if(deltbetacut[2] > 0.05  || deltbetacut[2] < -0.05) continue;    //Cut from Delta Beta vs Missingmass,Missing momentum, Invariantmass
    
    
    
    
   
    //------------------Correlation Theta-Phi, -----------------------/
    
    h_ThePhi[0]->Fill(myDataPERP->getEVNT_track(0).Phi()*TMath::RadToDeg(), myDataPERP->getEVNT_track(0).Theta()*TMath::RadToDeg());  
    h_ThePhi[1]->Fill(myDataPERP->getEVNT_track(1).Phi()*TMath::RadToDeg(), myDataPERP->getEVNT_track(1).Theta()*TMath::RadToDeg());
    h_ThePhi[2]->Fill(myDataPERP->getEVNT_track(2).Phi()*TMath::RadToDeg(), myDataPERP->getEVNT_track(2).Theta()*TMath::RadToDeg());
    
    
    //-----Cuts due to detector geometry (Fiduciary cuts)---------//
    //------------------------------------------------------------//

    Double_t phiproton_cut;
    phiproton_cut= myDataPERP->getEVNT_track(0).Phi()*TMath::RadToDeg();
    
    if( (phiproton_cut <= -25)   && (phiproton_cut >= -35)  ) continue;
    if( (phiproton_cut <= -85)   && (phiproton_cut >= -95)  ) continue;
    if( (phiproton_cut <= -145)  && (phiproton_cut >= -155) ) continue;
    if( (phiproton_cut >= 25)    && (phiproton_cut <= 35)   ) continue;
    if( (phiproton_cut >= 85)    && (phiproton_cut <= 95)   ) continue;
    if( (phiproton_cut >= 145)   && (phiproton_cut <= 155)  ) continue;
     
    h_ThePhicut[0]->Fill(phiproton_cut, myDataPERP->getEVNT_track(0).Theta()*TMath::RadToDeg());
    
    //kaon cuts
    
    Double_t phikaon_cut;
    phikaon_cut= myDataPERP->getEVNT_track(1).Phi()*TMath::RadToDeg();
    
    if( (phikaon_cut <= -25)   && (phikaon_cut >= -35)  ) continue;
    if( (phikaon_cut <= -85)   && (phikaon_cut >= -95)  ) continue;
    if( (phikaon_cut <= -145)  && (phikaon_cut >= -155) ) continue;
    if( (phikaon_cut >= 25)    && (phikaon_cut <= 35)   ) continue;
    if( (phikaon_cut >= 85)    && (phikaon_cut <= 95)   ) continue;
    if( (phikaon_cut >= 145)   && (phikaon_cut <= 155)  ) continue;
    
    h_ThePhicut[1]->Fill(phikaon_cut, myDataPERP->getEVNT_track(1).Theta()*TMath::RadToDeg());
    
    //Pion cuts
    
    Double_t phiPion_cut;
    phiPion_cut= myDataPERP->getEVNT_track(2).Phi()*TMath::RadToDeg();
    
    if( (phiPion_cut <= -25)   && (phiPion_cut >= -35)  ) continue;
    if( (phiPion_cut <= -85)   && (phiPion_cut >= -95)  ) continue;
    if( (phiPion_cut <= -145)  && (phiPion_cut >= -155) ) continue;
    if( (phiPion_cut >= 25)    && (phiPion_cut <= 35)   ) continue;
    if( (phiPion_cut >= 85)    && (phiPion_cut <= 95)   ) continue;
    if( (phiPion_cut >= 145)   && (phiPion_cut <= 155)  ) continue;
    
    h_ThePhicut[2]->Fill(phiPion_cut, myDataPERP->getEVNT_track(2).Theta()*TMath::RadToDeg());
    
    //------------------ Photons, Delta T  ------------------ // 
    
    for (int i=0;i<myDataPERP->getNum_photons();i++){
      //LOOP OVER ALL PHOTONS
      if (fabs(deltbeta[1])<0.025){
	h_DeltaTall[0]->Fill(myDataPERP->getDelt_t_k(i));
	h_DeltaTallvsp[0]->Fill(myDataPERP->getEVNT_track(1).Rho(),myDataPERP->getDelt_t_k(i));
      }
      if (fabs(deltbeta[2])<0.05){
	h_DeltaTall[1]->Fill(myDataPERP->getDelt_t_pi(i));
	h_DeltaTallvsp[1]->Fill(myDataPERP->getEVNT_track(2).Rho(),myDataPERP->getDelt_t_pi(i));
      }
    }
    
    //------------Delta T with Cuts ----------- //
    
    if (myDataPERP->getNumph_k()==1)
      h_DeltaT[0]->Fill(myDataPERP->getDelt_t_k(myDataPERP->getIndex_k(0)));
    if(myDataPERP->getNumph_pi()==1)
      h_DeltaT[1]->Fill(myDataPERP->getDelt_t_pi(myDataPERP->getIndex_pi(0)));
    
    if (myDataPERP->getNumph_k()!=1) continue;
    //if (myDataPERP->getNumph_pi()!=1) continue;

    //--------------- Energy loss ----------- //
    
    h_eloss[0]->Fill(1000.0*(myDataPERP->geteloss_track(0).Rho() - myDataPERP->getEVNT_track(0).Rho() ));
    h_eloss[1]->Fill(1000.0*(myDataPERP->geteloss_track(1).Rho() - myDataPERP->getEVNT_track(1).Rho() ));
    h_eloss[2]->Fill(1000.0*(myDataPERP->geteloss_track(2).Rho() - myDataPERP->getEVNT_track(2).Rho() ));


    //-------------- Reconstruction --------- //
    
    TLorentzVector photon, deuteron, kaon, kaonpion, proton, pion, Wneutron_kaon, Wneutron_pion, Sigma, Lambda, Neutron;
    photon.SetXYZM(0,0,myDataPERP->getTAGR_epho(myDataPERP->getIndex_k(0)),0);
    deuteron.SetXYZM(0,0,0,1.8756);
    double Px_kaonpion = myDataPERP->getEVNT_track(1).Rho()* sin(myDataPERP->getEVNT_track(1).Theta())* cos(myDataPERP->getEVNT_track(1).Phi());
    double Py_kaonpion = myDataPERP->getEVNT_track(1).Rho()* sin(myDataPERP->getEVNT_track(1).Theta())* sin(myDataPERP->getEVNT_track(1).Phi());
    double Pz_kaonpion = myDataPERP->getEVNT_track(1).Rho()*(myDataPERP->getEVNT_track(1).CosTheta());
    kaonpion.SetXYZM(Px_kaonpion, Py_kaonpion, Pz_kaonpion, 0.139);       // This mass is of Pion-, because we need remove the background of Pion-


    proton = myDataPERP->geteloss_track(0);
    kaon = myDataPERP->geteloss_track(1);
    pion = myDataPERP->geteloss_track(2);
    Wneutron_kaon = photon + deuteron - proton - kaon - pion;
    Neutron.SetXYZM(Wneutron_kaon.Px(), Wneutron_kaon.Py(), Wneutron_kaon.Pz(), 0.939);
    Sigma = pion + Neutron;
    Lambda = pion + proton;
    
    Wneutron_pion = photon + deuteron - proton - kaonpion - pion;         // This missing mass is with the Pion-


    h_MissingMass->Fill(Wneutron_kaon.M());
    h_MissingMass_kaonpion->Fill(Wneutron_pion.M());
    h_MissingPvsMass->Fill(Wneutron_kaon.M(),Wneutron_kaon.P());
    
    Double_t El = TMath::Power((Wneutron_kaon.M()-offsetx)*cos(angle)+(Wneutron_pion.M()-offsety)*sin(angle),2)/TMath::Power(radx,2)
      +TMath::Power((Wneutron_kaon.M()-offsetx)*sin(angle)-(Wneutron_pion.M()-offsety)*cos(angle),2)/TMath::Power(rady,2);
    
    if(El > 1) continue;
    
    
    h_MissingMasscut->Fill(Wneutron_kaon.M());
    h_MissingMass_kaonpioncut->Fill(Wneutron_pion.M());
    
    h_MissingP->Fill(Wneutron_kaon.P());
    
    h_MissingPvsSigmaMass->Fill(Sigma.M(), Wneutron_kaon.P());
    h_MissingMass_vsMissingMasskaonpion->Fill(Wneutron_kaon.M(), Wneutron_pion.M());
    
    if(Wneutron_kaon.P()<=0.2) continue;                                    //Cut for rescattering
    
    h_MissingPcut->Fill(Wneutron_kaon.P());
    
    h_InvariantMass->Fill(Sigma.M());

    h_LambdaMass->Fill(Lambda.M());

    if ( Lambda.M()>=1.11 && Lambda.M()<=1.122 ) continue;                   //Cut for LamdaMass in +/- 3sigma
    if ( Wneutron_kaon.M()<=0.9 || Wneutron_kaon.M()>=0.96 ) continue;       //Cut from correlation MM

    h_InvariantMasscut->Fill(Sigma.M());
    h_MissingMassvsSigmaMass->Fill(Sigma.M(), Wneutron_kaon.M());

    

    //if( ((Lambda.M()<1.05) || (Lambda.M()>1.20)) && (W.M()>0.9) && (W.M()<1.0) )
    //if( ((Lambda.M()<1.05) || (Lambda.M()>1.20)) && (W.M()>0.9) )
    
    
    //if(Sigma.M()<1.10 || Sigma.M()>1.40)
    
    // Pion - 
    h_DeltaBVSInvariantMass->Fill(Sigma.M(),deltbeta[2]);
    h_DeltaBVSMissingMass->Fill(Wneutron_kaon.M(),deltbeta[2]);
    h_DeltaBVSMissingMomentum->Fill(Wneutron_kaon.P(),deltbeta[2]);
    
  }
  cout<<endl;

  
   DoCanvas();
}

#endif
