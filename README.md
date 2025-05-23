# Running CI/CD for Embedded

## Author (of report)

```
Alban Favre
```

## Commands and stuff

**IMPORTANT:** due to Blinky being busted, I will work on the sensor instead

```bash
#Host
JLinkRemoteServerCLExe
#Tunnel version, doensn't work abandon it, use the other one with test and led as args
JLinkRemoteServerCLExe -UseTunnel -TunnelBySN -select="801000373"

#guest build, I use test instead of Debug to avoid setting the wifi up
cmake --list-presets
cmake --preset Test
cmake --build --preset app-test

#Guest
#my version
#based on previous lab
JRun --verbose --device RP2040_M0_0 --rtt -if SWD --pc off --sp off --ip "192.168.122.1" --args "sensor" build/Test/TSM_PicoW_Sensor.elf

```

- my serial number is:`801000373`

## 6.1.6 Assignment: GitHub Actions

```
IMPORTANT:
I could not make the wifi work (because I don't have a wifi right now)
I could not make the tunnel work: probably because it is not a good option
THERFORE: I will not use Debug, but Test
ALSO: I am using sensor because I busted the Blinky project
WHICH MEANS: I have moved and adapted the .github file, and will put the sensor test in the docker
```

```bash
#in the docker container:

JRun --verbose --device RP2040_M0_0 --rtt -if SWD --pc off --sp off --ip "192.168.122.1" --args "sensor" /project/build/Test/TSM_PicoW_Sensor.elf
# works

ctest --verbose --extra-verbose --test-dir /project/build/Test -R Sensor
# works
 
#but 
 
ctest --verbose --extra-verbose --test-dir /project/build/Test
 
#fails at Sensor, always, I dont know why
#My hypothesis is that I don't init the Sensor correctly when it is not ran alone, or that the led test dosen't end cpmpletely, which makes the sensor fail
```



### 1. Create a new GitHub repository.

Done: [https://github.com/alfavre/TSM_SoftEng_cicd_for_embedded_alban](https://github.com/alfavre/TSM_SoftEng_cicd_for_embedded_alban)

### 2. Push the project to GitHub.

Done

### 3. Inspect the deploy.yml GitHub Actions file. Check what it is doing.

it describes some workflow, In particular it describes the job: build-and-deploy which will be the pipeline for our github.

deploy.yml:

- define the names, permisson, env variable we will be using in the github pipeline
- the jobs we can run (only one for the assigenement build and deploy)
  - it builds the docker image in the github, and the deploys it (now we can use it in github, for tests).

### 4. On the GitHub web page, run the action manually, see figure 12.

![](img/lab_01.png)

### 5. Inspect the running job with the web front end: it should go through the steps (Figure 13) and create a Docker image and container (Figure 14).

See previous image

### 6. Edit the project CHANGELOG.md and add a new release version entry to it.

Done, it will be version 1.0.0

----

## Rendu

Use the steps in the lab 'Running CI/CD for Embedded' with setting up a CI/CD pipeline with GitHub Actions on GitHub.

Provide the URL to your git repository where you have completed the lab.

The git repository has to be your own repository, hosting the lab files can and commits, showing the  successful build and delivery of the RP2040 application.
