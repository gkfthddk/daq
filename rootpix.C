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
  printf("enter file name : ");
  scanf("%s",nrun);

  auto inf=new TFile(strcat(nrun,".root"),"read");
  auto intree=(TTree*) inf->Get("data");
  intree->SetBranchAddress("px",&px);
  intree->SetBranchAddress("py",&py);
  
  TCanvas *c1 = new TCanvas("c1", "KIMS", 800, 800);
  TH2F *hos1 = new TH2F("hos1", nrun, 100, 0, 100,100,0,100);

  hos1->Reset();
  
  for (i = 0; i < intree->GetEntries(); i++) {
    // read frame number
    intree->GetEntry(i);
    
    for(int j=0;j<100;j++){
    for(int k=0;k<100;k++){
      //ax=(signed char)px[j];
      //ay=(signed char)py[k];
      if(px[j]>19&&py[k]>19)pp[j][k]+=(px[j]+py[k]);//data read and cut noise
      //if(px[j]>9&&py[k]>9)pp[j][k]+=1;//data read and cut noise
    }
    }
    //printf("%d / %d filled, frame # = %d\n", i + 1, nsample, frame);
  }
  
  for(int j=0;j<100;j++){
    if(j==78)continue;
    if(j==59)continue;
    if(j==34)continue;
    if(j==0)continue;
    for(int k=0;k<100;k++){
    if(k==32)continue;
    if(k==72)continue;
    if(k==86)continue;
    if(k==0)continue;
      hos1->SetBinContent(j+1,k+1,pp[j][k]);//fill hist
    }
  }
 c1->cd(1);
 hos1->SetStats(0);
 hos1->Draw("colz");
t2=clock();
float diff((float)t2-(float)t1);
cout<<diff/CLOCKS_PER_SEC<<endl;//print running time
}
    
