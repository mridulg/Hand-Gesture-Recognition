for i=0:9
	for j=1:55
		path='/Users/mridul/HGR/Datasets/myDataset/';
		path;
		x=strcat(path,num2str(i),'_',num2str(j),'.png');
		x;
		img=imread(x);
		imshow(image);
	end
end
