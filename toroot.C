{
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
  float pp[100][100];
  short px[100];
  short py[100];

  printf("enter file name : ");
  scanf("%s", nrun);
  char rot[256];
  strcpy(rot,nrun);
  auto outf=new TFile(strcat(rot,".root"),"recreate");
  auto tree=new TTree("data","data");
  tree->SetDirectory(outf);
  tree->Branch("px",px,(std::string("px[")+std::to_string(100)+"]/S").c_str());
  tree->Branch("py",py,(std::string("py[")+std::to_string(100)+"]/S").c_str());
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
      px[ch] = (int)adcX;
    } 
    // fill Y channel
    for (ch = 0; ch <100; ch++){
      fscanf(fp, "%d", &adcY);
      py[ch] = (int)adcY;
    }
    
    tree->Fill();
    
    //printf("%d / %d filled, frame # = %d\n", i + 1, nsample, frame);
  }

  fclose(fp);
  
  tree->SetDirectory(outf);
  tree->GetCurrentFile()->Write("",TObject::kOverwrite);
  outf->Close();

}
    
