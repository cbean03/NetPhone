#include "liveMedia.hh"
#include "GroupsockHelper.hh"
#include "BasicUsageEnvironment.hh"


Boolean const isSSM = False;
UsageEnvironment* env;

// A structure to hold the state of the current session.
// It is used in the "afterPlaying()" function to clean up the session.
struct sessionState_t {
  FramedSource* source;
  RTPSink* sink;
  RTCPInstance* rtcpInstance;
  Groupsock* rtpGroupsock;
  Groupsock* rtcpGroupsock;
} sessionState;

//default file to send if one isn't provided
char const* inputFileName = "whistle.mp3";

void play(); // forward

int main(int argc, char** argv) {

//if another file name is provided, use this one
  if (argc > 1)
    inputFileName = argv[1]; 
  
  // Begin by setting up our usage environment:
  TaskScheduler* scheduler = BasicTaskScheduler::createNew();
  env = BasicUsageEnvironment::createNew(*scheduler);

  // Create 'groupsocks' for RTP and RTCP:
  char const* destinationAddressStr = "239.255.42.42";
  const unsigned short rtpPortNum = 6666;
  const unsigned short rtcpPortNum = rtpPortNum+1;
  const unsigned char ttl = 1; // low, in case routers don't admin scope

  struct in_addr destinationAddress;
  destinationAddress.s_addr = our_inet_addr(destinationAddressStr);
  const Port rtpPort(rtpPortNum);
  const Port rtcpPort(rtcpPortNum);

  sessionState.rtpGroupsock
    = new Groupsock(*env, destinationAddress, rtpPort, ttl);
  sessionState.rtcpGroupsock
    = new Groupsock(*env, destinationAddress, rtcpPort, ttl);

  // Create a 'MP3 RTP' sink from the RTP 'groupsock':
  sessionState.sink = MPEG1or2AudioRTPSink::createNew(*env, sessionState.rtpGroupsock);

  // Create (and start) a 'RTCP instance' for this RTP sink:
  const unsigned estimatedSessionBandwidth = 160; // in kbps; for RTCP b/w share
  const unsigned maxCNAMElen = 100;
  unsigned char CNAME[maxCNAMElen+1];
  gethostname((char*)CNAME, maxCNAMElen);
  CNAME[maxCNAMElen] = '\0'; // just in case
  sessionState.rtcpInstance
    = RTCPInstance::createNew(*env, sessionState.rtcpGroupsock,
			      estimatedSessionBandwidth, CNAME,
			      sessionState.sink, NULL /* we're a server */,
			      isSSM);

  play();

  env->taskScheduler().doEventLoop(); // does not return
  return 0; // only to prevent compiler warning
}

void afterPlaying(void* clientData); // forward

void play() {
  // Open the file as a 'MP3 file source':
  sessionState.source = MP3FileSource::createNew(*env, inputFileName);
  if (sessionState.source == NULL) {
    *env << "Unable to open file \"" << inputFileName
	 << "\" as a MP3 file source\n";
    exit(1);
  }

  // Finally, start the streaming:
  *env << "Beginning streaming...\n";
  sessionState.sink->startPlaying(*sessionState.source, afterPlaying, NULL);
}


void afterPlaying(void* /*clientData*/) {
  *env << "...done streaming\n";

  sessionState.sink->stopPlaying();

  // End this loop by closing the current source:
  Medium::close(sessionState.source);

  // And start another loop:
  play();
}
