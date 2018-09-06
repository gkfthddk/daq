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
  int dx,dy;
  int chek=0;
  int next=0;
  auto tree=new TTree("buf","buf");
  tree->Branch("dx",&dx,"dx/I");
  tree->Branch("dy",&dy,"dy/I");
  float vx[100],vy[100];
  //int pp[100][100];
  float **pp=new float*[100];
  for (int a=0;a<100;++a){
    pp[a]=new float[100];
    for (int b=0;b<100;++b){
      pp[a][b]=0.;
    }
  }
  short px[100];
  short py[100];

  auto inf=new TFile("frog.root","read");
  auto intree=(TTree*) inf->Get("data");
  intree->SetBranchAddress("px",&px);
  intree->SetBranchAddress("py",&py);
  
  TCanvas *c1 = new TCanvas("c1", "KIMS", 500, 1500);
  TH1F *hos1 = new TH1F("hos1", "h1", 100, 0, 100);
  TH1F *hos2 = new TH1F("hos2", "h2", 100, 0, 100);
  TH1F *hos3 = new TH1F("hos3", "h3", 100, 0, 100);

  hos1->Reset();
  hos2->Reset();
  hos3->Reset();
  int maxx,maxy;
  int idx,idy;
  float bx,by,bb,bv;
  //int idx[5],idy[5];
  int NN=intree->GetEntries();
  for (i = 0; i < NN; i++) {
    // read frame number
    intree->GetEntry(i);
    // fill X channel
    maxx=0;maxy=0;
    for(int j=0;j<100;j++){
      if(px[j]>0)vx[j]+=px[j];
      if(py[j]>0)vy[j]+=py[j];
        //hos1->SetBinContent(j+1,px[j]);
        //hos2->SetBinContent(j+1,py[j]);
        if(maxx<px[j]){maxx=px[j];idx=j;}
        if(maxy<py[j]){maxy=py[j];idy=j;}
      if(px[j]>30||py[j]>20){
        chek=1;
        break;
        }
    }
    if(chek==0)continue;
    intree->GetEntry(i-1);
    for(int j=0;j<100;j++){
      hos1->SetBinContent(j+1,px[j]);
    }
    intree->GetEntry(i);
    for(int j=0;j<100;j++){
      hos2->SetBinContent(j+1,px[j]);
    }
    intree->GetEntry(i+1);
    for(int j=0;j<100;j++){
      hos3->SetBinContent(j+1,px[j]);
    }
    
    ////int(distance(px,max_element(px,px+100)))
    if(chek==1){
      chek=0;
      hos2->Fit("gaus","Q","",idx-3,idx+3);
      bx=hos2->GetFunction("gaus")->GetParameter(1);
      //bb=hos3->GetBinContent(bx,by);
      bv=hos2->GetFunction("gaus")->GetMaximum();
      break;
      }
    /*if(chek==1){
      break;
    }*/
  }
 gPad->Clear();
 c1->Divide(1,3);
 c1->cd(1);
 hos1->Draw();
 //hos1->Fit("gaus","Q","",idx-5,idx+5);
 c1->cd(2);
 hos2->Draw();
 //hos2->Fit("gaus","Q","",idy-5,idy+5);
 c1->cd(3);
 hos3->Draw();
//int nfound=s->Search(hos10);
//cout<<hos1->GetFunction("gaus")->GetParameter(1)<<endl;
//cout<<hos2->GetFunction("gaus")->GetParameter(1)<<endl;
//c1->Update();
t2=clock();
float diff((float)t2-(float)t1);
cout<<diff/CLOCKS_PER_SEC<<endl;
}
    
