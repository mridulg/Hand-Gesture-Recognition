for i=0:9
	for j=1:55
		path='/Users/mridul/HGR/Datasets/myDataset/';
		x=strcat(path,num2str(i),'_',num2str(j),'.png');
		img=imread(x);
		scaled=imresize(image,[64 64]);
		destination=strcat('/Users/mridul/HGR/Datasets/scaledDataset/',num2str(i),'_', num2str(j), '.png');
		imwrite(scaled,destination);
	end
end

for i='a':'z'
	for j=1:55
		path='/Users/mridul/HGR/Datasets/myDataset/';
		x=strcat(path,i),'_',num2str(j),'.png');
		img=imread(x);
		scaled=imresize(image,[64 64]);
		destination=strcat('/Users/mridul/HGR/Datasets/scaledDataset/',i,'_', num2str(j), '.png');
		imwrite(scaled,destination);
	end
end
