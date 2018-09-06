{
  clock_t t1,t2;
  t1=clock();
  FILE *fp;
  char filename[256];
  char nrun[256];
  int sch;
  int nsample;
  int ch;
  int i;
  int frame;
  int adcX;
  int adcY;
  int ax,ay;
  //int pp[100][100];
  int **pp=new int*[100];
  for (int a=0;a<100;++a){
    pp[a]=new int[100];
    for (int b=0;b<100;++b){
      pp[a][b]=0;
    }
  }
  short px[100];
  short py[100];
  //printf("enter file name : ");
  //scanf("%s",nrun);
  strcpy(nrun,"plane");

  NNN=10000;
  auto inf=new TFile(strcat(nrun,".root"),"read");
  auto intree=(TTree*) inf->Get("data");
  intree->SetBranchAddress("px",&px);
  intree->SetBranchAddress("py",&py);
  
  TCanvas *c1 = new TCanvas("c1", "KIMS", 800, 800);
  TH2F *hos1 = new TH2F("hos1", nrun, 3,0,3,NNN, 0, NNN);
  TH2F *hos2 = new TH2F("hos2", nrun, 201, 0, 201,NNN,0,NNN);
  cout<<intree->GetEntries()<<endl;

  
  TH1F *fit1 = new TH1F("fit1", "hist", 100, 0, 100);
  TH1F *fit2 = new TH1F("fit2", "hist", 100, 0, 100);

  fit1->Reset();
  fit2->Reset();
  int maxx,maxy; 
  int idx,idy;
  float bx,by,bb,bv;
  int chek=0;
  int ychek=0;
  int xchek=0;
  int bx1=-1;
  int bx2=-1;
  int by1=-1;
  int by2=-1;
  for (i = 0; i < NNN; i++) {
    // read frame number
    intree->GetEntry(i);
    maxx=0;maxy=0;
    bx1=-1;
    bx2=-1;
    by1=-1;
    by2=-1;
    for(int j=0;j<100;j++){
      hos2->Fill(j,i,1);
      hos2->Fill(j+101,i,1);
    }
    for(int j=1;j<99;j++){

      if(px[j-1]>8&&px[j]>8&&px[j+1]>8){
        if(bx1 == -1){
          bx1=j;
          //j+=1;
        }else if(bx2 == -1){
          if(bx1<j-4){
            bx2=j;
            //j+=1;
          }else{
            bx1=-1;
            //j+=1;
          }
        }else{
          if(bx2<j-4){
          if(px[bx1]<px[bx2]&&px[bx1]<px[j]){
            bx1=j;
            //j+=1;
            //break;
          }else if(px[bx2]<px[bx1]&&px[bx2]<px[j]){
            bx2=j;
            //j+=1;
            //break;
          }
          }else{
            bx2=-1;
            //j+=1;
          }
        }
      }
      if(py[j-1]>8&&py[j]>8&&py[j+1]>8){
        if(by1 == -1){
          by1=j;
          j+=1;
        }else if(by2 == -1){
          if(by1<j-4){
            by2=j;
            j+=1;
          }else{
            by1=-1;
            j+=1;
          }
        }else{
          if(by2<j-4){
          if(py[by1]<py[by2]&&py[by1]<py[j]){
            by1=j;
            j+=1;
            break;
          }else if(py[by2]<py[by1]&&py[by2]<py[j]){
            by2=j;
            j+=1;
            break;
          }
          }else{
            by2=-1;
            j+=1;
          }
        }
      }
    } 
    
    for(int j=0;j<2;j++){
        if(bx1 == -1){
          break;
        }else{
          //cout<<(bx1-1)<<" "<<(bx1)<<" "<<(bx1+1)<<endl;
          hos2->Fill((bx1-1),i,1);
          hos2->Fill((bx1),i,1);
          hos2->Fill((bx1+1),i,1);
          bx1=bx2;
        }
        //hos2->Fill(101+j,i,2);
    }
    //for(int j=1;j<99;j++){

    //} 
    
    for(int j=0;j<2;j++){
        if(by1 == -1){
          break;
        }else{
          //cout<<(by1-1)<<" "<<(by1)<<" "<<(by1+1)<<endl;
          hos2->Fill((101+by1-1),i,1);
          hos2->Fill((101+by1),i,1);
          hos2->Fill((101+by1+1),i,1);
          by1=by2;
        }
        //hos2->Fill(101+j,i,2);
    }
      //if(px[j]>10&&j==20)hos1->Fill(i,px[j]);
      //
      //
      //
      /*
       * if(chek==0 || chek==1+0){
      if(j!=78&&j!=59&&j!=34&&j!=0){
      if(chek==0){if(px[j]>30)chek+=1;
      }
        fit1->SetBinContent(j+1,px[j]);
        if(maxx<px[j]){maxx=px[j];idx=j+1;}
      }
      if(j!=72&&j!=86&&j!=32&&j!=0){
      if(chek==0){if(py[j]>20)chek+=1;
      }
        fit2->SetBinContent(j+1,py[j]);
        if(maxy<py[j]){maxy=py[j];idy=j+1;}
      }
      }
      */

    
    /*if(chek==1){
      chek=0;
      fit1->Fit("gaus","Q","",idx-3,idx+3);
      fit2->Fit("gaus","Q","",idy-3,idy+3);
      bx=fit1->GetFunction("gaus")->GetParameter(1);
      by=fit2->GetFunction("gaus")->GetParameter(1);
      //bb=fit3->GetBinContent(bx,by);
      //bv=fit1->GetFunction("gaus")->GetMaximum();
      if(bx>=0&&by>=0&&bx<100&&by<100&&bx>idx-5&&bx<idx+5&&by>idy-5&&by<idy+5){
        bx=fit1->GetFunction("gaus")->GetMaximumX();
        by=fit2->GetFunction("gaus")->GetMaximumX();
        if(maxx*4>fit1->GetFunction("gaus")->GetMaximum()&&maxy*4>fit2->GetFunction("gaus")->GetMaximum()&&maxx/2<fit1->GetFunction("gaus")->GetMaximum()&&maxy/2<fit2->GetFunction("gaus")->GetMaximum()){
        hos1->Fill(2.,i,2);
      }
      }
      fit1->Reset();
      fit2->Reset();
      }*/
  }




    //printf("%d / %d filled, frame # = %d\n", i + 1, nsample, frame);
//hos2->SetBit(TH1::kNoStats);  
hos2->SetStats(0);  
hos2->GetXaxis()->SetTitle("Strip x :: Strip y");
hos2->GetYaxis()->SetTitle("Entry");
 c1->cd(1);
 hos2->Draw("colz");
t2=clock();
float diff((float)t2-(float)t1);
cout<<diff/CLOCKS_PER_SEC<<endl;//print running time
}
    
