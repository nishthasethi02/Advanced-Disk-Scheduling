#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

#define MAX_REQUESTS 100
#define MAX_TRACK 200

typedef struct {
    int request_id;
    int track_number;
    int arrival_time;
    int priority;
    bool is_deadline;
    int deadline;
} DiskRequest;

typedef struct {
    double confidence;
    int predicted_track;
} PredictionResult;

void log_performance(const char* algorithm, int movement);
void sort_requests(DiskRequest requests[], int n);

void fcfs(DiskRequest requests[], int n, int head);
void sstf(DiskRequest requests[], int n, int head);
void scan(DiskRequest requests[], int n, int head, int max_track);
void cscan(DiskRequest requests[], int n, int head, int max_track);
void look(DiskRequest requests[], int n, int head);
PredictionResult predict_next_request(DiskRequest history[], int history_size, int current_head);

void log_performance(const char* algorithm, int movement) {
    FILE *file = fopen("performance_log.txt", "a");
    if (file) {
        fprintf(file, "Algorithm: %s, Total Head Movement: %d\n", algorithm, movement);
        fclose(file);
    }
}

PredictionResult predict_next_request(DiskRequest history[], int history_size, int current_head) {
    PredictionResult result = {0.0, -1};
    
    if (history_size > 0) {
        result.predicted_track = history[history_size-1].track_number;
        result.confidence = 0.7; 
    }
    
    return result;
}

void sort_requests(DiskRequest requests[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            bool swap = false;
            if (requests[j].track_number > requests[j + 1].track_number) {
                swap = true;
            } else if (requests[j].track_number == requests[j + 1].track_number) {
                
                if (requests[j].priority < requests[j + 1].priority) {
                    swap = true;
                }
            }
            
            if (swap) {
                DiskRequest temp = requests[j];
                requests[j] = requests[j + 1];
                requests[j + 1] = temp;
            }
        }
    }
}

void fcfs(DiskRequest requests[], int n, int head) {
    int movement = 0;
    printf("FCFS Order: %d", head);
    
    for (int i = 0; i < n; i++) {
        movement += abs(head - requests[i].track_number);
        printf(" -> %d", requests[i].track_number);
        head = requests[i].track_number;
    }
    
    printf("\nTotal Head Movement: %d\n", movement);
    log_performance("FCFS", movement);
}

void sstf(DiskRequest requests[], int n, int head) {
    int movement = 0;
    bool visited[MAX_REQUESTS] = {false};
    printf("SSTF Order: %d", head);

    for (int completed = 0; completed < n; completed++) {
        int min_dist = INT_MAX, index = -1;
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                int dist = abs(head - requests[i].track_number);
               
                if (dist < min_dist || 
                    (dist == min_dist && requests[i].priority > requests[index].priority)) {
                    min_dist = dist;
                    index = i;
                }
            }
        }
        
        movement += min_dist;
        visited[index] = true;
        head = requests[index].track_number;
        printf(" -> %d", head);
    }
    
    printf("\nTotal Head Movement: %d\n", movement);
    log_performance("SSTF", movement);
}

void scan(DiskRequest requests[], int n, int head, int max_track) {
   
    DiskRequest sorted_requests[MAX_REQUESTS];
    memcpy(sorted_requests, requests, n * sizeof(DiskRequest));
    
    sort_requests(sorted_requests, n);
    
    int movement = 0, pos = 0;
    
    while (pos < n && sorted_requests[pos].track_number < head) pos++;
    
    printf("SCAN Order: %d", head);

    for (int i = pos; i < n; i++) {
        movement += abs(head - sorted_requests[i].track_number);
        head = sorted_requests[i].track_number;
        printf(" -> %d", head);
    }
    
    movement += max_track - head;
    printf(" -> %d", max_track);
    head = max_track;
    
    for (int i = pos - 1; i >= 0; i--) {
        movement += abs(head - sorted_requests[i].track_number);
        head = sorted_requests[i].track_number;
        printf(" -> %d", head);
    }
    
    printf("\nTotal Head Movement: %d\n", movement);
    log_performance("SCAN", movement);
}

void cscan(DiskRequest requests[], int n, int head, int max_track) {
   
    DiskRequest sorted_requests[MAX_REQUESTS];
    memcpy(sorted_requests, requests, n * sizeof(DiskRequest));
    
    sort_requests(sorted_requests, n);
    
    int movement = 0, pos = 0;
    
    while (pos < n && sorted_requests[pos].track_number < head) pos++;
    
    printf("C-SCAN Order: %d", head);

    for (int i = pos; i < n; i++) {
        movement += abs(head - sorted_requests[i].track_number);
        head = sorted_requests[i].track_number;
        printf(" -> %d", head);
    }
    
    movement += max_track - head;
    printf(" -> %d", max_track);
    head = 0;
    movement += max_track;
    printf(" -> 0");
    
    for (int i = 0; i < pos; i++) {
        movement += abs(head - sorted_requests[i].track_number);
        head = sorted_requests[i].track_number;
        printf(" -> %d", head);
    }
    
    printf("\nTotal Head Movement: %d\n", movement);
    log_performance("C-SCAN", movement);
}

void look(DiskRequest requests[], int n, int head) {
    
    DiskRequest sorted_requests[MAX_REQUESTS];
    memcpy(sorted_requests, requests, n * sizeof(DiskRequest));
    
    sort_requests(sorted_requests, n);
    
    int movement = 0, pos = 0;
    
    while (pos < n && sorted_requests[pos].track_number < head) pos++;
    
    printf("LOOK Order: %d", head);

    for (int i = pos; i < n; i++) {
        movement += abs(head - sorted_requests[i].track_number);
        head = sorted_requests[i].track_number;
        printf(" -> %d", head);
    }
    
    for (int i = pos - 1; i >= 0; i--) {
        movement += abs(head - sorted_requests[i].track_number);
        head = sorted_requests[i].track_number;
        printf(" -> %d", head);
    }
    
    printf("\nTotal Head Movement: %d\n", movement);
    log_performance("LOOK", movement);
}

int main() {
  
        DiskRequest requests[] = {
            {1, 98, 50, 2, false, 0},
            {2, 186, 75, 1, true, 200},
            {3, 37, 120, 3, false, 0},
            {4, 122, 90, 2, true, 180},
            {5, 14, 60, 1, false, 0},
            {6, 215, 130, 3, false, 0},
            {7, 61, 100, 2, true, 220},
            {8, 199, 140, 1, false, 0}
        };
    
    int n = sizeof(requests) / sizeof(requests[0]);
    int head = 53;
    int max_track = MAX_TRACK;

    PredictionResult prediction = predict_next_request(requests, n, head);
    printf("ML Prediction: Track %d (Confidence: %.2f)\n\n", 
           prediction.predicted_track, prediction.confidence);

    printf("FCFS Algorithm:\n");
    fcfs(requests, n, head);
    
    printf("\nSSTF Algorithm:\n");
    sstf(requests, n, head);
    
    printf("\nSCAN Algorithm:\n");
    scan(requests, n, head, max_track);
    
    printf("\nC-SCAN Algorithm:\n");
    cscan(requests, n, head, max_track);
    
    printf("\nLOOK Algorithm:\n");
    look(requests, n, head);

    return 0;
}