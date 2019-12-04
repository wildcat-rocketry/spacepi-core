plugins {
    java
    application
}

repositories {
    mavenCentral()
}

dependencies {
    implementation("org.eclipse.paho:org.eclipse.paho.client.mqttv3:1.2.2")
}

application {
    mainClassName = "com.gitlab.ffaero.tools.spacepi.support.debugclient.Main"
}
