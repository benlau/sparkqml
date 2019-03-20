FROM ubuntu:18.04

VOLUME ["/src"]

# USERID should be same as the current user
ARG USERID
ARG USERNAME=docker

WORKDIR /home/$USERNAME

ENV DISPLAY=":0"

RUN useradd --create-home --no-user-group -u $USERID $USERNAME -s /bin/bash && adduser $USERNAME sudo

RUN apt-get update && \
	apt-get install -y --no-install-recommends python patchelf \
	mesa-common-dev file sudo \
	build-essential ca-certificates wget libfontconfig1 libx11-xcb1 \
	git libpulse-mainloop-glib0 libgl1-mesa-dev && \
	rm -rf /var/lib/apt/lists/* && \
    apt-get clean 

RUN wget https://raw.githubusercontent.com/benlau/qtci/master/bin/extract-qt-installer -O /usr/local/bin/extract-qt-installer && \
	wget https://raw.githubusercontent.com/Larpon/linuxdeployqt.py/master/linuxdeployqt.py -O /usr/local/bin/linuxdeployqt.py && \
	wget https://raw.githubusercontent.com/benlau/qtci/master/recipes/install-qt-online -O /usr/local/bin/install-qt-online && \
	wget https://www.qpm.io/download/v0.11.0/linux_386/qpm -O /usr/local/bin/qpm && \
	chmod 755 /usr/local/bin/linuxdeployqt.py  /usr/local/bin/qpm  /usr/local/bin/extract-qt-installer /usr/local/bin/install-qt-online && \
	mkdir -p /usr/local/lib/appimagetool && \
	wget https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage -O /usr/local/lib/appimagetool/appimagetool && \
	cd /usr/local/lib/appimagetool && \
	chmod u+x appimagetool && \
	./appimagetool --appimage-extract && \
	ln -s /usr/local/lib/appimagetool/squashfs-root/AppRun /usr/local/bin/appimagetool && \
	chmod -R o+rx  /usr/local/lib/appimagetool

RUN VERBOSE=true install-qt-online 'qt.qt5.5122.gcc_64,qt.qt5.5122.qtvirtualkeyboard' /opt

ENV PATH="/opt/Qt/5.12.2/gcc_64/bin:${PATH}"

RUN echo 'export PATH=/opt/Qt/5.12.2/gcc_64/bin:${PATH}' >> /home/docker/.bashrc

USER $USERNAME

CMD ["bash"]

