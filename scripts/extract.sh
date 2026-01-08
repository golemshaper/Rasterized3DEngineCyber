OUTPUT="./tmp/"
mkdir -p ${OUTPUT}
CONTAINER_ID=$(docker create golemshaper/build:latest --)
echo "Extracting files from ${CONTAINER_ID} into ${OUTPUT}"
docker cp ${CONTAINER_ID}:/app/. ${OUTPUT}
docker rm ${CONTAINER_ID}