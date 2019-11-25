#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <spacepi.h>

void callback_1(void *context, const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain);
void callback_2(void *context, const char *channel, const void *data, size_t data_len, spacepi_qos_t qos, int retain);

int main(int argc, const char **argv) {
    CHECK_ERROR(spacepi_pubsub_init);
    CHECK_ERROR(spacepi_pubsub_cleanup);
    CHECK_ERROR(spacepi_pubsub_init);
    puts("Connected.");
    char a = 'A', b = 'B';
    CHECK_ERROR(spacepi_subscribe, "/abc/123", exactly_once, callback_1, &a);
    CHECK_ERROR(spacepi_subscribe, "/abc/123", exactly_once, callback_1, &b);
    CHECK_ERROR(spacepi_subscribe, "/abc/123", exactly_once, callback_2, &a);
    CHECK_ERROR(spacepi_subscribe, "/abc/123", exactly_once, callback_2, &b);
    puts("Testing multiple subscriptions (expected 1A, 1B, 2A, 2B)");
    CHECK_ERROR(spacepi_publish, "/abc/123", NULL, 0, exactly_once, FALSE);
    sleep(1);
    puts("Ubsubscribing based on function...");
    CHECK_ERROR(spacepi_unsubscribe, "/abc/123", callback_1, NULL);
    puts("Testing unsubscription and republish (expected 2A, 2B)");
    CHECK_ERROR(spacepi_publish, "/abc/123", NULL, 0, exactly_once, FALSE);
    sleep(1);
    puts("Unsubscribing based on context...");
    CHECK_ERROR(spacepi_unsubscribe, "/abc/123", NULL, &a);
    puts("Testing unsubscription and republish (expected 2B)");
    CHECK_ERROR(spacepi_publish, "/abc/123", NULL, 0, exactly_once, FALSE);
    sleep(1);
    puts("Unsubscribing all...");
    CHECK_ERROR(spacepi_unsubscribe, "/abc/123", NULL, NULL);
    puts("Testing unsubscription and republish (expected none)");
    CHECK_ERROR(spacepi_publish, "/abc/123", NULL, 0, exactly_once, FALSE);
    sleep(1);
    puts("Testing complete.");
    CHECK_ERROR(spacepi_pubsub_cleanup);
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
