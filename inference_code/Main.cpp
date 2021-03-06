/*******************************************************************************
* Copyright 2018 Intel Corporation
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/
#include <getopt.h> 
#include <sys/time.h>
#include "Model.hpp"

using namespace std;

int main(int argc, char **argv) {
	const char *weight_path = NULL;
    int batch_size = 1, loop = 1;
    int ch;
    while ((ch = getopt(argc, argv, "Hi:b:l:h:w:W:")) != -1) {
        switch (ch) {
			case 'W': weight_path = optarg; break;
            case 'b': batch_size = atoi(optarg); break;
            case 'l': loop = atoi(optarg); break;
            case 'H': 
				printf("USAGE: %s [options <parameter>]\n", argv[0]);
				printf("options:\n");
				printf("\t-W <weight file>\t\t\"default:weights.bin\"\n");
				printf("\t-b <batch size>\t\t\t\"default:1\"\n");
				printf("\t-l <loop number>\t\t\"default:1\"\n");
				exit(0);
        }
    } 

    // Init
    float *fake_input, *last_output;
    Model model(weight_path, batch_size);

    fake_input = model.make_fake_input();

    // Execute
    printf("\nrunning ....\n");
    struct timeval start, end;
    gettimeofday(&start, NULL);
    for (int l = 0; l < loop; l++) {
        last_output = model.inference(fake_input);
    }
    gettimeofday(&end, NULL);
    float time_cost = 1.0 * ((end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000) / loop;

    // Print of all
    model.print_output();
    
    // Print the last
    printf("------------------------------------\n");
    printf("Last_output >> [%f %f %f %f %f %f %f %f %f %f]\n", last_output[0], last_output[1], last_output[2], last_output[3], last_output[4], last_output[5], last_output[6], last_output[7], last_output[8], last_output[9]);
    printf("\nAVG Time: %.2f ms\n\n", time_cost);

    return 0;
}
