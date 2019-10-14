FROM ubuntu:19.04

RUN apt-get update -y \
 && apt-get upgrade -y \
 && apt-get install -y ninja-build cmake \
                       clang clang-format clang-tidy g++ \
                       git wget unzip shellcheck libfl-dev libssl-dev \
                       curl libcurl4-openssl-dev sqlite3 libsqlite3-dev \
                       openjdk-8-jdk-headless openjdk-12-jdk-headless \
 && update-java-alternatives --set java-1.8.0-openjdk-amd64

ARG GRADLE_VERSION=5.6.2
ENV GRADLE_HOME /opt/gradle
RUN mkdir -p ${GRADLE_HOME} \
 && cd ${GRADLE_HOME} \
 && wget https://downloads.gradle-dn.com/distributions/gradle-${GRADLE_VERSION}-bin.zip \
 && unzip gradle-${GRADLE_VERSION}-bin.zip \
 && rm gradle-${GRADLE_VERSION}-bin.zip \
 && mv gradle-${GRADLE_VERSION}/* .
ENV PATH ${PATH}:${GRADLE_HOME}/bin

ARG ANDROID_VERSION=4333796
ENV ANDROID_HOME /opt/android-sdk
RUN mkdir -p ${ANDROID_HOME} \
 && cd ${ANDROID_HOME} \
 && wget https://dl.google.com/android/repository/sdk-tools-linux-${ANDROID_VERSION}.zip \
 && unzip sdk-tools-linux-${ANDROID_VERSION}.zip \
 && rm sdk-tools-linux-${ANDROID_VERSION}.zip
ENV PATH ${PATH}:${ANDROID_HOME}/tools
ENV PATH ${PATH}:${ANDROID_HOME}/platform-tools
ENV PATH ${PATH}:${ANDROID_HOME}/tools/bin

RUN yes | sdkmanager --licenses
