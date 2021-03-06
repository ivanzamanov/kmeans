REUTERS_DIR=data/reuters21578/
EXTRACT_DIR=data/extracted
STEM_DIR=data/stemmed

set -e

if [ ! -d $EXTRACT_DIR ]; then
  mkdir -p $EXTRACT_DIR
fi

if [ ! -d $STEM_DIR ]; then
  mkdir -p $STEM_DIR
fi

ARTICLE_COUNT=0
for file in $REUTERS_DIR/*sgm
do
  EXTRACT_OUTPUT="`./bin/extractor $file $EXTRACT_DIR $ARTICLE_COUNT`"
  ARTICLE_COUNT=$(($ARTICLE_COUNT+${EXTRACT_OUTPUT##*:}))
  echo "Articles: $ARTICLE_COUNT"
done
echo "Total articles: $ARTICLE_COUNT"

echo "Tokenizing and stemming"
COUNT=0
for file in `ls $EXTRACT_DIR`
do
	if [ ! -f $STEM_DIR/$file ]; then
#		TOKENIZED="`./bin/tokenizer < $EXTRACT_DIR/$file`"
#		./bin/stemmer > $STEM_DIR/$file <<< "$TOKENIZED"
		./bin/tokenizer < $EXTRACT_DIR/$file > $STEM_DIR/$file
		COUNT=$(($COUNT + 1))
		if [ $(($COUNT % 100)) == 0 ]; then
			echo "$COUNT"
		fi
	fi
done

echo "Clustering"
./bin/kmeans $STEM_DIR
