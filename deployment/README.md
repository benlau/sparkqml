

Build docker image
```
docker build --build-arg USERID=`id -u` -t sparkqml-builder .
```

Build AppImage
```
docker run --rm  -i --name sparkqml-builder -v ${PWD}/..:/src -t sparkqml-builder bash -c "cd /src; ./deployment/create-appimage-in-docker.sh"
```

Test docker image
```
docker run --rm  -i --name sparkqml-builder -v ${PWD}/..:/src -t sparkqml-builder /bin/sh
```


