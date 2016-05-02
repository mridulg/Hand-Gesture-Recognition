import os
filenames = next(os.walk("/Users/mridul/Downloads/handgesturedataset_part3/"))[2]
for i in filenames:
	bleh=i
	x=i.split("_")
	y=[]
	num=0
	# print len(x)
	for j in range(len(x)):
		# print x[j]
		y.append(x[j])
	# print y
	# print "Y1 is "+str(y[1])
	name=str(y[1])+"_"
	if str(y[2])=="dif":
		num=int(y[4])+50
	elif str(y[2])=="bot":
		num=int(y[4])+55
	elif str(y[2])=="left":
		num=int(y[4])+60
	elif str(y[2])=="right":
		num=int(y[4])+65
	elif str(y[2])=="top":
		num=int(y[4])+70
	name=name+str(num)+".png"
	print name
	os.rename("/Users/mridul/Downloads/handgesturedataset_part3/"+i,"/Users/mridul/Downloads/handgesturedataset_part1/"+name)

