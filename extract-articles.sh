REUTERS_DIR=data/reuters21578/
EXTRACT_DIR=/tmp/extracted

if [ ! -d $EXTRACT_DIR ]; then
  mkdir -p $EXTRACT_DIR
fi

ARTICLE_COUNT=0
for file in $REUTERS_DIR/*sgm
do
  EXTRACT_OUTPUT="`./bin/extractor $file $EXTRACT_DIR $ARTICLE_COUNT`"
  ARTICLE_COUNT=$(($ARTICLE_COUNT+${EXTRACT_OUTPUT##*:}))
  echo "Articles: $ARTICLE_COUNT"
done

echo "Total articles: $ARTICLE_COUNT"
