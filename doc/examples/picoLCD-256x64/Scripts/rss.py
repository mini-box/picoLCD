import feedparser
import datetime

# lcd4linux permits only 1 parameter passed to the function
# we send the rss title id with the ! spacer
def pf(rssfeed):
	output = ' '
	titleid = 0
	realfeed = rssfeed.split('!')[0]
	titleid = int(rssfeed.split('!')[-1])
	
	info = feedparser.parse(realfeed);

	#for entry in info.entries:
	#	output += entry.title
	
	#entry = info.entries[titleid]
	return  info.entries[titleid].title



#print pf("http://slashdot.org/slashdot.rdf!10")

