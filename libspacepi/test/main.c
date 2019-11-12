#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <spacepi.h>

void callback_1(void *context, const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain);
void callback_2(void *context, const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain);

int main(int argc, const char **argv) {
    int e;
    if ((e = spacepi_pubsub_init())) {
        if (e > sys_nerr) {
            fprintf(stderr, "spacepi_pubsub_init: %d\n", ~e);
        } else {
            perror("spacepi_pubsub_init");
        }
        return e;
    }
    if ((e = spacepi_pubsub_cleanup())) {
        if (e > sys_nerr) {
            fprintf(stderr, "spacepi_pubsub_cleanup: %d\n", ~e);
        } else {
            perror("spacepi_pubsub_cleanup");
        }
        return e;
    }
    sleep(1);
    if ((e = spacepi_pubsub_init())) {
        if (e > sys_nerr) {
            fprintf(stderr, "spacepi_pubsub_init: %d\n", ~e);
        } else {
            perror("spacepi_pubsub_init");
        }
        return e;
    }
    sleep(1);
    puts("Connected.");
    char a = 'A', b = 'B';
    if ((e = spacepi_subscribe("/abc/123", exactly_once, callback_1, &a))) {
        if (e > sys_nerr) {
            fprintf(stderr, "spacepi_subscribe: %d\n", ~e);
        } else {
            perror("spacepi_subscribe");
        }
        return e;
    }
    if ((e = spacepi_subscribe("/abc/123", exactly_once, callback_1, &b))) {
        if (e > sys_nerr) {
            fprintf(stderr, "spacepi_subscribe: %d\n", ~e);
        } else {
            perror("spacepi_subscribe");
        }
        return e;
    }
    if ((e = spacepi_subscribe("/abc/123", exactly_once, callback_2, &a))) {
        if (e > sys_nerr) {
            fprintf(stderr, "spacepi_subscribe: %d\n", ~e);
        } else {
            perror("spacepi_subscribe");
        }
        return e;
    }
    if ((e = spacepi_subscribe("/abc/123", exactly_once, callback_2, &b))) {
        if (e > sys_nerr) {
            fprintf(stderr, "spacepi_subscribe: %d\n", ~e);
        } else {
            perror("spacepi_subscribe");
        }
        return e;
    }
    puts("Testing multiple subscriptions (expected 1A, 1B, 2A, 2B)");
    if ((e = spacepi_publish("/abc/123", NULL, 0, exactly_once, FALSE))) {
        if (e > sys_nerr) {
            fprintf(stderr, "spacepi_publish: %d\n", ~e);
        } else {
            perror("spacepi_publish");
        }
        return e;
    }
    sleep(1);
    puts("Ubsubscribing based on function...");
    if ((e = spacepi_unsubscribe("/abc/123", callback_1, NULL))) {
        if (e > sys_nerr) {
            fprintf(stderr, "spacepi_unsubscribe: %d\n", ~e);
        } else {
            perror("spacepi_unsubscribe");
        }
        return e;
    }
    puts("Testing unsubscription and republish (expected 2A, 2B)");
    if ((e = spacepi_publish("/abc/123", NULL, 0, exactly_once, FALSE))) {
        if (e > sys_nerr) {
            fprintf(stderr, "spacepi_publish: %d\n", ~e);
        } else {
            perror("spacepi_publish");
        }
        return e;
    }
    sleep(1);
    puts("Unsubscribing based on context...");
    if ((e = spacepi_unsubscribe("/abc/123", NULL, &a))) {
        if (e > sys_nerr) {
            fprintf(stderr, "spacepi_unsubscribe: %d\n", ~e);
        } else {
            perror("spacepi_unsubscribe");
        }
        return e;
    }
    puts("Testing unsubscription and republish (expected 2B)");
    if ((e = spacepi_publish("/abc/123", NULL, 0, exactly_once, FALSE))) {
        if (e > sys_nerr) {
            fprintf(stderr, "spacepi_publish: %d\n", ~e);
        } else {
            perror("spacepi_publish");
        }
        return e;
    }
    sleep(1);
    puts("Unsubscribing all...");
    if ((e = spacepi_unsubscribe("/abc/123", NULL, NULL))) {
        if (e > sys_nerr) {
            fprintf(stderr, "spacepi_unsubscribe: %d\n", ~e);
        } else {
            perror("spacepi_unsubscribe");
        }
        return e;
    }
    puts("Testing unsubscription and republish (expected none)");
    if ((e = spacepi_publish("/abc/123", NULL, 0, exactly_once, FALSE))) {
        if (e > sys_nerr) {
            fprintf(stderr, "spacepi_publish: %d\n", ~e);
        } else {
            perror("spacepi_publish");
        }
        return e;
    }
    sleep(1);
    puts("Testing complete.");
    if ((e = spacepi_pubsub_cleanup())) {
        if (e > sys_nerr) {
            fprintf(stderr, "spacepi_pubsub_cleanup: %d\n", ~e);
        } else {
            perror("spacepi_pubsub_cleanup");
        }
        return e;
    }
    return EXIT_SUCCESS;
}

void callback_1(void *context, const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain) {
    char c = *(char *) context;
    printf("Got response from 1%c\n", c);
}

void callback_2(void *context, const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain) {
    char c = *(char *) context;
    printf("Got response from 2%c\n", c);
}
