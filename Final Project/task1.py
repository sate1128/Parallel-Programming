import json
import math
import time

start = time.time()
data = {}
with open('token.txt') as data_file:  
    data = data_file.readlines()
data = [x.strip() for x in data] 

for i in range(len(data)):
	data[i] = data[i].split()
N = 304713


words='i wanted challenge notions classic beauty arrangement patience license expect think time like there blood in brain impressive bigger than whole house shaky thought would just go with stand think look split second bingo put knee into groin coffee table between wall got one foot that thing herself gun case thing had a glass door could hear it breaking down enough before with this small caliber handgun of taking kickbacks anybody knows goddamn lie all right tear apart come out alive fellow wear shoes trouser watch wait find weakness far only scouts have had reports small farther north over supposed kill then kill cop believe been lying long one more time hurt yeah pictures thinking cameras sensors probably put bomb water three weeks later swimming people story hear science every standard lived life fact utterly beside point true matter because prove real you are entitled to your opinion we feel quite strongly that it would by both irresponsible naive human unknown completely uncontrollable situation absolutely defenseless champeen camp hell bars and seven cold drinks in fifteen minutes eat busted bottles and rusty nails any damn thing so kindly oblige as to let me cut off your yankee head sugar and cream take mine black absolutely correct politicians want panic headlines spoiling the festival love sidekick punching address spend my favorite kitchen gears detective going discipline operation'
s_word=words.split(' ')
l1=list(set(s_word))
l2=['shoot', 'game', 'straight', 'pool', 'corner', 'height', 'move', 'fantasy', 'pocket', 'far', 'favorite', 'shiny', 'happy', 'fits', 'rage', 'consider', 'fifty', 'thousand', 'dollars', 'word', 'outside', 'commit', 'compass', 'economic', 'poor', 'dogfish', 'porker', 'sand', 'shark', 'suicide', 'sailing', 'pleasure', 'boating', 'close', 'friend', 'brain', 'return', 'address', 'the', 'envelope', 'realize', 'need', 'debit', 'card', 'programming', 'team', 'middle', 'night', 'blind', 'wife', 'touch', 'emotional', 'investment', 'circumstances', 'gentlemen', 'today', 'personally', 'jumpy', 'delivered', 'very', 'figure', 'bumper', 'sticker', 'overrun', 'passionate', 'exactly', 'truck', 'driver', 'survived', 'first', 'last', 'hellride', 'marshmallow', 'rice', 'squares', 'trip', 'real', 'simple', 'sugarplum', 'right', 'medication', 'britches', 'negotiate', 'key', 'understand', 'million', 'football', 'behind', 'know', 'four', 'chords', 'only', 'desire', 'main', 'column', 'people', 'around', 'turkey', 'anyway', 'eat', 'interupt', 'wig', 'answer', 'hope']
l3=sorted(list(set(l1+l2)))


tf=[]
#sentence = ['oblige', 'oblige', 'bomb']
for v in range(300):#300
	count_sum = 0
	now = l3[v]
	print(v)
	for i in data:
		count = 0
		for j in range(len(i)):#sentences in tokens 
			if now == i[j]:
				count = 1
		count_sum+=count 
	#print(count_sum)
	tf.append(math.log10(N/count_sum))



f2= open('task1.txt','r', encoding = 'windows-1252')
content = f2.read()
f2.close()
content = content.split('\n')
task = []
for i in content:
	task.append(i.split(' '))
cc=[]
for index,sen in enumerate(task):
	cc.append([])
	for those in l3:
		count=0
		for ww in sen:
			if(those == ww):
				count+=1
		cc[index].append(count)
	index+=1
result = []
result.append(l3)
result.append(tf)
for i in range(10):
	result.append([])
	for j in range(300):
		result[i+2].append(cc[i][j]*tf[j])
print(result)

output = ''
for i in range(300):
	output+=result[0][i]
	output+='|'
output+='\n'
for i in range(300):
	output+=str(result[1][i])
	output+=','
output+='\n'
for i in range(2,12):
	for j in range(300):
		output+=str(result[i][j])
		output+=','
	output+='\n'
with open("Output.txt", "w") as text_file:
	print(output, file=text_file)

end = time.time()

print("It cost %f sec" % (end - start))