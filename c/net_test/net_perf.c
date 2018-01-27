#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <stdarg.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>

const char kUsageTemplate[] =
    "Usage: %s --server --port=PORT PACKET...\n"
    "       %s --client ADDR PORT\n"
    "       %s --help\n"
    "Tests network performance.\n\n"
    "Packets are described in the following format: size/delay/count.\n"
    "  size        size of packet in KB\n"
    "  delay       delay in ms\n"
    "  count       number of packets\n\n"
    "Options:\n"
    "  -p, --port=PORT            server port\n"
    "  -h, --help                 print this message and exit\n";

static const struct option kLongOpts[] = {
    {"client", 0, 0, 'C'},
    {"server", 0, 0, 'S'},
    {"port", 1, 0, 'p'},
	{"help", 0, 0, 'h'},
	{0, 0, 0, 0}
};

static const char kShortOpts[] = "+p:h";

#define kMaxPacketDesc 128

typedef struct PacketDescriptionTag {
    int size;
    int interval;
    int count;
    unsigned char * data;
} PacketDescription;

typedef struct SettingsTag {
    int isClient;
    int isServer;
    int isHelp;
    int port;
    const char * addr;
    int fps;
    int packetDescCount;
    PacketDescription packetDesc[kMaxPacketDesc];
} Settings;

const char * kProgName = 0;

static int ParseCommandLine(int argc, char * argv[], Settings * settings);
static int ParsePacketDescription(const char * desc, PacketDescription * description);

void ErrorMsg(const char * src);
long long GetTimeOfDayInMs();
int Sleep(unsigned ms);
static int WriteAll(int sd, const void * data, int toWrite);
static int ReadAll(int sd, void * data, int toRead);
static int DummyReadAll(int sd, int toRead);

static int ServerMain(const Settings * settings);
static int CreateServerSocket(const Settings * settings);
static int ProcessClient(int sd, const Settings * settings);

static int ClientMain(const Settings * settings);
static int ConnectToServer(const Settings * settings);

int main(int argc, char * argv[]) {
    Settings settings;
    if( ParseCommandLine(argc, argv, &settings) < 0 )
        return EXIT_FAILURE;

    if( settings.isServer )
        return ServerMain(&settings);
    else if( settings.isClient )
        return ClientMain(&settings);
    else
        return EXIT_FAILURE;
}

int ParseCommandLine(int argc, char * argv[], Settings * settings) {
    memset(settings, 0, sizeof(Settings));
    kProgName = argv[0];
    settings->addr = "";
    int c;
    do {
        c = getopt_long(argc, argv, kShortOpts, kLongOpts, NULL);
        switch( c ) {
        case -1:
            break;
        case 'C':
            settings->isServer = 0;
            settings->isClient = 1;
            break;
        case 'S':
            settings->isServer = 1;
            settings->isClient = 0;
            break;
        case 'p':
            settings->port = atoi(optarg);
            break;
        case 'h':
            settings->isHelp = 1;
            break;
        default:
            fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
            return -1;
        }
    } while( -1 != c );

    if( settings->isHelp ||
        !(settings->isServer || settings->isClient) ||
        (settings->isClient && argc - optind != 2) ||
        (settings->isServer && argc == optind)
    ) {
        fprintf(stderr, kUsageTemplate, argv[0], argv[0], argv[0]);
        return -1;
    }

    if( settings->isClient ) {
        settings->addr = argv[optind];
        settings->port = atoi(argv[optind + 1]);
        return 0;
    }

    int i;
    for( i = optind; i < argc; ++i ) {
        if( settings->packetDescCount >= kMaxPacketDesc ) {
            fprintf(stderr, "%s: too many packet descriptions\n", argv[i]);
            return -1;
        }

        if( ParsePacketDescription(argv[i], &settings->packetDesc[settings->packetDescCount]) < 0 ) {
            fprintf(stderr, "%s: failed to parse packet description -- '%s'\n", argv[0], argv[i]);
            return -1;
        }
        ++settings->packetDescCount;
    }

    return 0;
}

static int ParsePacketDescription(const char * str, PacketDescription * desc) {
    const char * p1 = NULL;
    const char * p2 = NULL;

    p1 = strchr(str, '/');
    if( NULL == p1 )
        return -1;

    p2 = strchr(p1 + 1, '/');
    if( NULL == p2 )
        return -1;

    desc->size = atoi(str) * 1024;
    desc->interval = atoi(p1 + 1);
    desc->count = atoi(p2 + 1);

    if( desc->size <= 0 || desc->interval <=0 || desc->count <= 0 )
        return -1;


    desc->data = (unsigned char *)malloc(desc->size);
    if( NULL == desc->data )
        return -1;

    memset(desc->data, '*', desc->size);

    return 0;
}

void ErrorMsg(const char * src) {
    fprintf(stderr, "%s: %s failed: %s\n", kProgName, src, strerror(errno));
}

long long GetTimeOfDayInMs() {
    struct timeval now;
    if( gettimeofday(&now, NULL) < 0 ) {
        ErrorMsg("gettimeofday()");
        return -1;
    }

    return now.tv_sec * 1000LL + now.tv_usec / 1000LL;
}

int Sleep(unsigned ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = 1000000LL * (ms - 1000 * ts.tv_sec);

    if( nanosleep(&ts, NULL) < 0 ) {
        ErrorMsg("nanosleep()");
        return -1;
    }

    return 0;
}

static int WriteAll(int sd, const void * data, int toWrite) {
    int wasWritten = 0, actualWritten = 0;

    while( wasWritten != toWrite ) {
        actualWritten = write(sd, (const unsigned char *)data + wasWritten, toWrite - wasWritten);
        if( actualWritten <= 0 ) {
            ErrorMsg("write()");
            return -1;
        }
        wasWritten += actualWritten;
    }

    return 0;
}

static int ReadAll(int sd, void * data, int toRead) {
    int wasRead = 0, actualRead = 0;
    while( wasRead != toRead ) {
        actualRead = read(sd, (unsigned char *)data + wasRead, toRead - wasRead);
        if( actualRead < 0 ) {
            ErrorMsg("read()");
            return -1;
        } else if( 0 == actualRead ) {
            return -1;
        }

        wasRead += actualRead;
    }

    return 0;
}

static int DummyReadAll(int sd, int toRead) {
    unsigned char buf[1024];
    int bufSize = sizeof(buf);
    int n = toRead / bufSize;
    int i;

    for( i = 0; i < n; ++i ) {
        if( ReadAll(sd, buf, bufSize) < 0 )
            return -1;
    }

    return ReadAll(sd, buf, toRead % bufSize);
}

static int ServerMain(const Settings * settings) {
    int lsd = 0, asd = 0;
    struct sockaddr_in cliaddr;
    socklen_t clilen = 0;

    signal(SIGPIPE, SIG_IGN);

    lsd = CreateServerSocket(settings);
    if( lsd < 0 )
        return EXIT_FAILURE;

    for( ; ; ) {
        asd = accept(lsd, (struct sockaddr *)&cliaddr, &clilen);
        if( asd < 0 ) {
            ErrorMsg("accept()");
            close(lsd);
            return EXIT_FAILURE;
        }

        printf("Accept new connection from %s\n", inet_ntoa(cliaddr.sin_addr));

        if( ProcessClient(asd, settings) < 0 )
            return EXIT_FAILURE;

        close(asd);
    }

    return EXIT_SUCCESS;
}

static int CreateServerSocket(const Settings * settings) {
    int sd;
    int yes = 1;

    struct sockaddr_in servaddr;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if( sd < 0 ) {
        ErrorMsg("socket()");
        return -1;
    }

    if( setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0 ) {
        close(sd);
        ErrorMsg("setsockopt()");
        return -1;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(settings->port);
    if( bind(sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) {
        close(sd);
        ErrorMsg("bind()");
        return -1;
    }

    if( listen(sd, 1024) < 0 ) {
        close(sd);
        ErrorMsg("listen()");
        return -1;
    }

    return sd;
}

static int ProcessClient(int sd, const Settings * settings) {
    long long  prevTime, curTime;
    unsigned td = 0;
    int i, j;
    const PacketDescription * desc = NULL;

    prevTime = GetTimeOfDayInMs();
    if( prevTime < 0 )
        return -1;

    for( ; ; ) {
        for( i = 0; i < settings->packetDescCount; ++i ) {
            desc = &settings->packetDesc[i];
            printf("Process: %d/%d/%d\n", desc->size, desc->interval, desc->count);
            for( j = 0; j < desc->count; ++j ) {
                curTime = GetTimeOfDayInMs();
                if( curTime < 0 )
                    return -1;

                td = curTime - prevTime;

                if( WriteAll(sd, &td, sizeof(td)) < 0 )
                    return 0;

                if( WriteAll(sd, &desc->size, sizeof(desc->size))  < 0)
                    return 0;

                if( WriteAll(sd, desc->data, desc->size)  < 0)
                    return 0;

                if( Sleep(desc->interval) < 0 )
                    return -1;

                prevTime = curTime;
            }
        }
    }
}

int max_delta = 0;
unsigned frame_count = 0;
long long start_time = 0;
int last_deltas[20] = {0};
int worst_second_drift = 0;
int worst_second_start = 0;

void ClientInterrupted(int arg) {
    long long avg = (GetTimeOfDayInMs() - start_time) / frame_count;
    printf("\nMax interframe time: %d; Avg: %d\n", max_delta, avg);
    printf("\nWorst second duration: %d (at frame #%d)\n", worst_second_drift, worst_second_start);
    exit(0);
}


static int ClientMain(const Settings * settings) {
    int sd;
    int td, actualTd;
    unsigned dataSize;
    long long prevTime, curTime;

    sd = ConnectToServer(settings);
    if( sd < 0 )
        return EXIT_FAILURE;

    prevTime = GetTimeOfDayInMs();
    if( prevTime < 0 )
        return EXIT_FAILURE;

    signal(SIGINT, ClientInterrupted);

    printf("#%-10s %-10s %-10s %-10s %-10s\n", "Frame", "Expected", "Measured", "Delta", "Size");

    start_time = GetTimeOfDayInMs();
    for( ; ; ) {
        if( ReadAll(sd, &td, sizeof(td)) < 0 )
            break;

        if( ReadAll(sd, &dataSize, sizeof(dataSize)) < 0 )
            break;

        if( DummyReadAll(sd, dataSize) < 0 )
            break;

        ++frame_count;

        curTime = GetTimeOfDayInMs();
        if( curTime < 0 )
            return EXIT_FAILURE;

        actualTd = curTime - prevTime;
        printf("#%-10d %-10d %-10d %-10d %-10d\n", frame_count, td, actualTd, td - actualTd, dataSize);
        prevTime = curTime;

        if (frame_count > 10 && actualTd > max_delta)
            max_delta = actualTd;

        int sum = 0;
        int i = 0;
        for (i = 0; i < 20; ++i) {
            sum += last_deltas[i];
        }
        if (sum > worst_second_drift) {
            worst_second_drift = sum;
            worst_second_start = frame_count;
        }

        last_deltas[frame_count % 20] = actualTd;
    }

    close(sd);

    return EXIT_FAILURE;
}

static int ConnectToServer(const Settings * settings) {
    int sd;
    struct sockaddr_in servaddr;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if( sd < 0 ) {
        ErrorMsg("socket()");
        return -1;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(settings->port);
    if( inet_pton(AF_INET, settings->addr, &servaddr.sin_addr) <= 0 ) {
        ErrorMsg("inet_pton()");
        close(sd);
        return -1;
    }

    if( connect(sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ) {
        ErrorMsg("connect()");
        close(sd);
        return -1;
    }

    return sd;
}
