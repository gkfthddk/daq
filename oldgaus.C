{
  float sign(float x){return(x>0.)-(x<0.);}
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
  int xchek=0;
  int ychek=0;
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
  printf("enter file name : ");
  scanf("%s",nrun);

  auto inf=new TFile(strcat(nrun,".root"),"read");
  //auto inf=new TFile("frog.root","read");
  auto intree=(TTree*) inf->Get("data");
  intree->SetBranchAddress("px",&px);
  intree->SetBranchAddress("py",&py);
  
  TCanvas *c1 = new TCanvas("c1", "KIMS", 500, 500);
  c1->Divide(2,1);
  res=10;
  TH1F *hos1 = new TH1F("hos1", "hist", 100, 0, 100);
  TH1F *hos2 = new TH1F("hos2", "hist", 100, 0, 100);
  TH2F *hos3 = new TH2F("hos3", nrun, res*100, 0, 100,res*100,0,100);

  hos1->Reset();
  hos2->Reset();
  hos3->Reset();
  int maxx,maxy,sigx,sigy;
  int count=0;
  int idx,idy;
  float bx,by,bb,bv;
  //int idx[5],idy[5];
  int NN=intree->GetEntries();
  for (i = 0; i < NN/1; i++) {
  //for (i = 447556; i < NN*3.21/5; i++) {
  //for (i = NN*1/2; i < NN*3/5; i++) {
    // read frame number
    intree->GetEntry(i);
    xchek=0;ychek=0;
    // fill X channel
    maxx=0;maxy=0;
    for(int j=0;j<100;j++){
      if(px[j]>0)vx[j]+=px[j];
      if(py[j]>0)vy[j]+=py[j];
      if(xchek==0 || xchek==1+0){
      if(ychek==0 || ychek==1+0){
        
      if(j!=78&&j!=59&&j!=34&&j!=0){
      if(xchek==0){if(px[j]>30)xchek+=1;
      }
        hos1->SetBinContent(j+1,px[j]);
        if(maxx<px[j]){maxx=px[j];idx=j+1;}
      }
      if(j!=72&&j!=86&&j!=32&&j!=0){
      if(ychek==0){if(py[j]>20)ychek+=1;
      }
        hos2->SetBinContent(j+1,py[j]);
        if(maxy<py[j]){maxy=py[j];idy=j+1;}
      }
      }
      }
    }
    ////int(distance(px,max_element(px,px+100)))
    if(xchek==1&&ychek==1){
      xchek=1;
      ychek=1;
      hos1->Fit("gaus","Q","",idx-3,idx+3);
      hos2->Fit("gaus","Q","",idy-3,idy+3);
      bx=hos1->GetFunction("gaus")->GetParameter(1);
      by=hos2->GetFunction("gaus")->GetParameter(1);
      //bb=hos3->GetBinContent(bx,by);
      //bv=hos1->GetFunction("gaus")->GetMaximum();
      if(bx>=0&&by>=0&&bx<100&&by<100&&bx>idx-5&&bx<idx+5&&by>idy-5&&by<idy+5){
        bx=hos1->GetFunction("gaus")->GetMaximumX();
        by=hos2->GetFunction("gaus")->GetMaximumX();
        if(maxx*4>hos1->GetFunction("gaus")->GetMaximum()&&maxy*4>hos2->GetFunction("gaus")->GetMaximum()&&maxx/2<hos1->GetFunction("gaus")->GetMaximum()&&maxy/2<hos2->GetFunction("gaus")->GetMaximum()){
        //if(maxx*4>hos1->GetFunction("gaus")->Eval(idx)&&maxy*4>hos2->GetFunction("gaus")->Eval(idy)&&maxx/2<hos1->GetFunction("gaus")->Eval(idx)&&maxy/2<hos2->GetFunction("gaus")->Eval(idy)){
        //if(idy==97)cout<<i<<" "<<maxy<<" "<<hos2->GetFunction("gaus")->Eval(idy)<<endl;
        maxx=hos1->GetMaximum();
        maxy=hos2->GetMaximum();
        for(float l=-2.;l<2.+1./res;l+=1./res){
        for(float m=-2.;m<2.+1./res;m+=1./res){
            if(m==0){
              //pp[idx+l][idy+m]+=1.*maxy/maxx*hos1->GetFunction("gaus")->Eval(idx+l);
              hos3->Fill(bx+l,by+m,1.*maxy/maxx*hos1->GetFunction("gaus")->Eval(bx+l));
            }
            else if(l==0){
              //pp[idx+l][idy+m]+=hos2->GetFunction("gaus")->Eval(idy+m);
              hos3->Fill(bx+l,by+m,1.*hos2->GetFunction("gaus")->Eval(by+m));
            }
            else{
              //pp[idx+l][idy+m]+=(1.*maxy/maxx*hos1->GetFunction("gaus")->Eval(idx+l)+hos2->GetFunction("gaus")->Eval(idy+m))/2.;
              bv=sqrt(m*m+l*l);
              hos3->Fill(bx+l,by+m,(1.*maxy/maxx*hos1->GetFunction("gaus")->Eval(bx+bv)+hos2->GetFunction("gaus")->Eval(by+bv))/2.);
              }
        }
        }
      //cout<<i<<" x "<<bx<<" y "<<by<<" value "<<" x "<<hos1->GetFunction("gaus")->GetMaximum()<<" y "<<maxy<<endl;
/*        if(bx>60&&bx<64&&by>72&&by<78){
        cout<<i<<endl;
        break;
        }*/
        //hos3->Reset();
        count+=1;
      }
      }
      hos1->Reset();
      hos2->Reset();
      }
    /*if(chek==1){
      break;
    }*/
  }
/*for(int j=0;j<100;j++){
for(int k=0;k<100;k++){
hos3->SetBinContent(j+1,k+1,pp[j][k]);
}}*/
 c1->cd(1);
 //hos1->Draw();
 //hos1->Fit("gaus","Q","",idx-5,idx+5);
 //hos3->SetBinContent(hos3->GetMaximumBin(),0);
 hos3->SetStats(0);
 hos3->Draw("colz");
 c1->cd(2);
 //hos2->Draw();
 //hos2->Fit("gaus","Q","",idy-5,idy+5);
//int nfound=s->Search(hos10);
//cout<<hos1->GetFunction("gaus")->GetParameter(1)<<endl;
//cout<<hos2->GetFunction("gaus")->GetParameter(1)<<endl;
c1->Update();
t2=clock();
float diff((float)t2-(float)t1);
cout<<count<<endl;
cout<<diff/CLOCKS_PER_SEC<<endl;
}
    
