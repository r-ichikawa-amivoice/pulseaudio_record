// Copyright (c) 2021, Advanced Media.
// All rights reserved.
//
// ソースコード形式かバイナリ形式か、変更するかしないかを問わず、再頒布および使用が許可されます。
// 
// 本ソフトウェアは、著作権者およびコントリビューターによって「現状のまま」提供されており、明示黙示を問わず、商業的な使用可能性、および特定の目的に対する適合性に関する暗黙の保証も含め、またそれに限定されない、いかなる保証もありません。著作権者もコントリビューターも、事由のいかんを問わず、 損害発生の原因いかんを問わず、かつ責任の根拠が契約であるか厳格責任であるか（過失その他の）不法行為であるかを問わず、仮にそのような損害が発生する可能性を知らされていたとしても、本ソフトウェアの使用によって発生した（代替品または代用サービスの調達、使用の喪失、データの喪失、利益の喪失、業務の中断も含め、またそれに限定されない）直接損害、間接損害、偶発的な損害、特別損害、懲罰的損害、または結果損害について、一切責任を負わないものとします。
//
// 自分用に作ったサンプルです。
// エラー処理をほとんど入れていないです。 
//
// g++ main.cpp -lpulse
// 
// Usage: ./a.out
// 

#include <stdio.h>
#include <unistd.h>

#include <pulse/pulseaudio.h>

void pa_state_cb(pa_context *c, void *pa_th_ml) {
	pa_context_state_t state;
	state = pa_context_get_state(c);
	//printf("state:%d\n", state);
	pa_threaded_mainloop_signal((pa_threaded_mainloop*)pa_th_ml, 0);
}

static void stream_read_cb(pa_stream *s, size_t length, void *userdata){
	const void *data;
	pa_stream_peek(s, &data, &length);
	write(STDOUT_FILENO, data, length);
	pa_stream_drop(s);
}

int main(int ac,char *av[]){

	pa_threaded_mainloop *pa_th_ml = pa_threaded_mainloop_new();
	if(NULL==pa_th_ml){
		//fprintf(stderr,"Cannot create main loop.\n");
		return 1;
	}

	pa_context *pa_ctx = pa_context_new(pa_threaded_mainloop_get_api(pa_th_ml),"PulseAudio_test");
	if(NULL==pa_ctx){
		//fprintf(stderr,"Cannot create context.\n");
		return 1;
	}

	//printf("Mainloop and Context Created.\n");

	pa_context_set_state_callback(pa_ctx, pa_state_cb, pa_th_ml);
	pa_threaded_mainloop_lock(pa_th_ml);
	pa_threaded_mainloop_start(pa_th_ml);
	pa_context_connect(pa_ctx, NULL, PA_CONTEXT_NOAUTOSPAWN, NULL);

	while (1) {
		pa_context_state_t pa_ctx_state = pa_context_get_state(pa_ctx);
		assert(PA_CONTEXT_IS_GOOD(pa_ctx_state));
		if (pa_ctx_state == PA_CONTEXT_READY) break;
		pa_threaded_mainloop_wait(pa_th_ml);
	}

	const pa_sample_spec ss={
		PA_SAMPLE_S16LE,
		16000,
		1
	};

	pa_stream *pa_rec_stream = pa_stream_new(pa_ctx,"Record",&ss,NULL);
	if(NULL != pa_rec_stream){
		//printf("Stream created!  Getting there!\n");
	}

	pa_stream_set_read_callback(pa_rec_stream, stream_read_cb, NULL);
	int pa_rec_stream_res = pa_stream_connect_record(pa_rec_stream, NULL, NULL, (pa_stream_flags_t)0);

	if(pa_rec_stream_res < 0){
		//printf("pa_stream_connect_record failed\n");
	}

	pa_threaded_mainloop_unlock(pa_th_ml);

	getchar();

	pa_threaded_mainloop_stop(pa_th_ml);

	pa_stream_disconnect(pa_rec_stream);
	pa_stream_unref(pa_rec_stream);
	pa_context_disconnect(pa_ctx);
	pa_context_unref(pa_ctx);
	pa_threaded_mainloop_free(pa_th_ml);

}
