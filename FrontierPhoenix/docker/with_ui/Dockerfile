FROM frontier-dev:latest

# qt
RUN pip3 install aqtinstall~=3.0.2

ARG QT=5.15.2
ARG QT_MODULES=all
ARG QT_HOST=linux
ARG QT_TARGET=desktop
ARG QT_ARCH=""

RUN aqt install --outputdir /opt/qt ${QT} ${QT_HOST} ${QT_TARGET} ${QT_ARCH}     -m ${QT_MODULES}
ENV PATH /opt/qt/${QT}/gcc_64/bin:$PATH
ENV QT_PLUGIN_PATH /opt/qt/${QT}/gcc_64/plugins/
ENV QML_IMPORT_PATH /opt/qt/${QT}/gcc_64/qml/
ENV QML2_IMPORT_PATH /opt/qt/${QT}/gcc_64/qml/

RUN apt install mesa-common-dev libglu1-mesa-dev -y


RUN apt-get install libqt5waylandclient5 -y
WORKDIR /

RUN echo '#!/bin/bash' > /entrypoint.sh
#RUN echo 'echo "DISPLAY=$DISPLAY XAUTH_ADD_ARG=$XAUTH_ADD_ARG"' >> /entrypoint.sh
RUN echo 'xauth add $DISPLAY $XAUTH_ADD_ARG' >> /entrypoint.sh
RUN echo 'exec $@' >> /entrypoint.sh
RUN chmod +x /entrypoint.sh
RUN cat /entrypoint.sh

ENTRYPOINT [ "/entrypoint.sh" ]
CMD ["/bin/bash"]
