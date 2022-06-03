void Decode(){
	cout<<"Decode(filename)"<<endl;
	cout<<"DecodeTime(filename)"<<endl;
}
void Decode(string filename, TString outfile){
	FILE* fbin;
	fbin=fopen(filename.data(),"rb");
	int nev=0,qdcnev=0,tdcnev=0;
	int qdc[16];
	int tdc[16];


	const int32_t header = 0xffffffff;
	const int32_t footer = 0xfffffffe;
	const int32_t eof = 0xfffffffd;

	TFile* file = new TFile(outfile,"recreate");
	TTree* tree = new TTree("tree","tree");
	tree->Branch("nev",&nev,"nev/I");
	tree->Branch("qdcnev",&qdcnev,"qdcnev/I");
	tree->Branch("tdcnev",&tdcnev,"tdcnev/I");
	tree->Branch("qdc",qdc,"qdc[16]/I");
	tree->Branch("tdc",tdc,"tdc[16]/I");

	int32_t data[34];
	bool decoding = true;	
	bool assign = true;	
	int32_t dat;
	int nqdc=-1;
	int ntdc=-1;
	int ndat=0;
	int32_t QDC=0xFFFFFFF0;
	int32_t TDC=0xFFFFFFF1;
	bool qflag=false,tflag=false;
	while(decoding){
		fread(&dat,sizeof(int32_t),1,fbin);
		//		cout<<"Data : "<<std::hex<<dat<<endl;	
		if(dat==eof){
			cout<<"File End:"<<endl;
			decoding=false;
			break;
		}
		else if(dat==header){
			//			cout<<"Header"<<endl;
			nqdc=-1;
			ntdc=-1;
			ndat=0;
			assign = false;
		}
		else if(dat== footer){
			//			cout<<"Footer"<<endl;
			assign = true;	
			qflag=false;
			tflag=false;
		}
		else if(dat==QDC){
			//				qdcnev= dat;
			qflag=true;
		}
		else if(dat==TDC){
			//				tdcnev= dat;
			qflag=false;
			tflag=true;
		}
		else{
			data[ndat]=dat;
			ndat++;
			if(qflag){
				//				cout<<"QDC"<<nqdc<<endl;
				if(nqdc==-1){
					qdcnev=dat;
				}
				else {
					qdc[nqdc]=dat;
				}
				nqdc++;
			}
			else if(tflag){
				//				cout<<"TDC"<<ntdc<<endl;
				if(ntdc==-1){
					tdcnev=dat;
				}
				else {
					tdc[ntdc]=dat;
				}
				ntdc++;
			}
		}
		if(assign){
			//			cout<<"Assigning data..."<<endl;
			nev=data[0];
			tree->Fill();
			//	cout<<"Tree Filled"<<endl;
		}
	}
	file->Write();
}
void Read(TString filename){
	TFile* file = new TFile(filename,"read");
	TTree* tree = (TTree*)file->Get("tree");
	int nev,qdcnev,tdcnev;
	int qdc[16],tdc[16];
	tree->SetBranchAddress("nev",&nev);
	tree->SetBranchAddress("qdcnev",&qdcnev);
	tree->SetBranchAddress("tdcnev",&tdcnev);
	tree->SetBranchAddress("qdc",qdc);
	tree->SetBranchAddress("tdc",tdc);
	int ref=1,ch1 = 2,ch2=3;
	double tb = 0.025;//ns
	TCanvas* c1 = new TCanvas("c1","c1",1200,600);
	c1->Divide(2,2);
	TH1D* ht1 = new TH1D("TDC_1","TDC_1",1000,-1000,0);
	TH1D* ht2 = new TH1D("TDC_2","TDC_2",100,-1000,0);
	TH1D* htd = new TH1D("TDC_d","TDC_d",100,-300,300);
	TH2D* hph1 = new TH2D("QVT1","QVT1",100,0,1000,100,-500,500);
	TCanvas* c2 = new TCanvas("c2","c2",1200,600);
	c2->Divide(2,2);
	TH1D* ha1 = new TH1D("QDC_1","QDC_1",100,0,1000);
	TH1D* ha2 = new TH1D("QDC_2","QDC_2",100,0,1000);
	TH2D* hac = new TH2D("Q1:Q2","Q1:Q2",100,0,1000,100,0,1000);
	TH2D* hph2 = new TH2D("QVT2","QVT2",100,0,1000,100,-500,500);
	
	TCanvas* c3 = new TCanvas("c3","c3",1200,600);
	TH1I* ev = new TH1I("evd","evd",200,-100,100);
	for(int i=0;i<tree->GetEntries();i++){
		tree->GetEntry(i);
		double t1 = tdc[ch1]-tdc[ref];
		double t2 = tdc[ch2]-tdc[ref];
		int nt = tdcnev;
		tree->GetEntry(i);
		double a1 = qdc[ch1];
		double a2 = qdc[ch2];
		int nq = qdcnev;
		ht1->Fill(t1);
		ht2->Fill(t2);
		htd->Fill(t2-t1);
		ha1->Fill(a1);
		ha2->Fill(a2);
		hac->Fill(a1,a2);
		hph1->Fill(a1,t2-t1);
		hph2->Fill(a2,t1-t2);
		ev->Fill(nq-nt);
	}
	c1->cd(1);
	ht1->Draw();
	c1->cd(2);
	ht2->Draw();
	c1->cd(3);
	htd->Draw();
	htd->Fit("gaus");
	c1->cd(4);
	hph1->Draw("col");
	c2->cd(1);
	ha1->Draw();
	c2->cd(2);
	ha2->Draw();
	c2->cd(3);
	hac->Draw("col");
	c2->cd(4);
	hph2->Draw("col");
	c3->cd();
	ev->Draw();
}
void DecodeTime(string filename, TString outfile){
	FILE* fbin;
	fbin=fopen(filename.data(),"rb");


	const int32_t header = 0xffffffff;
	const int32_t footer = 0xfffffffe;
	const int32_t eof = 0xfffffffd;

	

	int32_t qt;
	int32_t tt;
	int32_t et;
	int32_t wt;
	TFile* file = new TFile(outfile,"recreate");
	TTree* tree = new TTree("tree","tree");
	tree->Branch("qt",&qt,"qt/I");
	tree->Branch("tt",&tt,"tt/I");
	tree->Branch("et",&et,"et/I");
	tree->Branch("wt",&wt,"wt/I");

	int32_t data[34];
	bool decoding = true;	
	bool assign = true;	
	int32_t dat;
	int ndat=0;
	while(decoding){
		fread(&dat,sizeof(int32_t),1,fbin);
		if(dat==eof){
			cout<<"File End:"<<endl;
			decoding=false;
			break;
		}
		else if(dat==header){
			nqdc=-1;
			ntdc=-1;
			ndat=0;
			assign = false;
		}
		else if(dat== footer){
			assign = true;	
		}
		else{
			data[ndat]=dat;
			ndat++;
		}
		if(assign){
			qt=data[0];
			tt=data[1];
			et=data[2];
			wt=data[3];
			tree->Fill();
		}
	}
	file->Write();
}
void Read(TString filename){
	TFile* file = new TFile(filename,"read");
	TTree* tree = (TTree*)file->Get("tree");
	int nev,qdcnev,tdcnev;
	int qdc[16],tdc[16];
	tree->SetBranchAddress("nev",&nev);
	tree->SetBranchAddress("qdcnev",&qdcnev);
	tree->SetBranchAddress("tdcnev",&tdcnev);
	tree->SetBranchAddress("qdc",qdc);
	tree->SetBranchAddress("tdc",tdc);
	int ref=1,ch1 = 2,ch2=3;
	double tb = 0.025;//ns
	TCanvas* c1 = new TCanvas("c1","c1",1200,600);
	c1->Divide(2,2);
	TH1D* ht1 = new TH1D("TDC_1","TDC_1",1000,-1000,0);
	TH1D* ht2 = new TH1D("TDC_2","TDC_2",100,-1000,0);
	TH1D* htd = new TH1D("TDC_d","TDC_d",100,-300,300);
	TH2D* hph1 = new TH2D("QVT1","QVT1",100,0,1000,100,-500,500);
	TCanvas* c2 = new TCanvas("c2","c2",1200,600);
	c2->Divide(2,2);
	TH1D* ha1 = new TH1D("QDC_1","QDC_1",100,0,1000);
	TH1D* ha2 = new TH1D("QDC_2","QDC_2",100,0,1000);
	TH2D* hac = new TH2D("Q1:Q2","Q1:Q2",100,0,1000,100,0,1000);
	TH2D* hph2 = new TH2D("QVT2","QVT2",100,0,1000,100,-500,500);
	
	TCanvas* c3 = new TCanvas("c3","c3",1200,600);
	TH1I* ev = new TH1I("evd","evd",200,-100,100);
	for(int i=0;i<tree->GetEntries();i++){
		tree->GetEntry(i);
		double t1 = tdc[ch1]-tdc[ref];
		double t2 = tdc[ch2]-tdc[ref];
		int nt = tdcnev;
		tree->GetEntry(i);
		double a1 = qdc[ch1];
		double a2 = qdc[ch2];
		int nq = qdcnev;
		ht1->Fill(t1);
		ht2->Fill(t2);
		htd->Fill(t2-t1);
		ha1->Fill(a1);
		ha2->Fill(a2);
		hac->Fill(a1,a2);
		hph1->Fill(a1,t2-t1);
		hph2->Fill(a2,t1-t2);
		ev->Fill(nq-nt);
	}
	c1->cd(1);
	ht1->Draw();
	c1->cd(2);
	ht2->Draw();
	c1->cd(3);
	htd->Draw();
	htd->Fit("gaus");
	c1->cd(4);
	hph1->Draw("col");
	c2->cd(1);
	ha1->Draw();
	c2->cd(2);
	ha2->Draw();
	c2->cd(3);
	hac->Draw("col");
	c2->cd(4);
	hph2->Draw("col");
	c3->cd();
	ev->Draw();
}
