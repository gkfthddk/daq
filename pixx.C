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
  float px[100];
  float py[100];
  //float pp[100][100];
  float **pp=new float*[100];
   for (int a=0;a<100;++a){
     pp[a]=new float[100];
     for (int b=0;b<100;++b){
       pp[a][b]=0.;
     }
   }

  int dx,dy;

  printf("enter file name : ");
  scanf("%s", nrun);

  fp=fopen(strcat(nrun,".txt"),"rt");

  FILE *process;
  char buff[1024];
  char command[100];
  strcpy(command,"wc -l < ");
  process = popen(strcat(command,nrun),"r");
  if(process!=NULL){
    while(!feof(process)){
      fgets(buff,sizeof(buff),process);
    }
    pclose(process);
  }
  int line=(atoi(buff)-1)/201;
  fscanf(fp, "%d", &nsample);
  
  TCanvas *c1 = new TCanvas("c1", "KIMS", 800, 800);
  TH2F *hos1 = new TH2F("hos1", "hist", 100, 0, 100,100,0,100);

  hos1->Reset();
  
  cout<<"sig : "<<line<<endl;
  for (i = 0; i < line/1; i++) {
    // read frame number
    fscanf(fp, "%d", &frame);
   
    // fill X channel
    for (ch = 0; ch < 100; ch++){
      fscanf(fp, "%d", &adcX);
      px[ch] = adcX;
    } 
    // fill Y channel
    for (ch = 0; ch <100; ch++){
      fscanf(fp, "%d", &adcY);
      py[ch] = adcY;
    }
    
    for (int j =0;j<100;j++){
      for(int k=0;k<100;k++){
        if(px[j]>50&&py[k]>50){
          pp[j][k]+=(px[j]+py[k])/2;
        }
      }
    }
    
    //pp[int(distance(px,max_element(px,px+100)))][int(distance(py,max_element(py,py+100)))]+=1;
    //dx=int(distance(px,max_element(px,px+100)));
    //dy=int(distance(py,max_element(py,py+100)));
    //tree->Fill();
    
    //printf("%d / %d filled, frame # = %d\n", i + 1, nsample, frame);
  }
  for (int j =0;j<100;j++){
    for(int k=0;k<100;k++){
      hos1->SetBinContent(j,k,pp[j][k]);
    }
  } 

  fclose(fp);

  c1->cd(1);
  hos1->Draw("colz");

  c1->Modified();
  c1->Update();
  
  //tree->SetDirectory(outf);
  //tree->GetCurrentFile()->Write("",TObject::kOverwrite);
  //outf->Close();
t2=clock();
float diff((float)t2-(float)t1);
cout<<diff/CLOCKS_PER_SEC<<endl;
}
    
