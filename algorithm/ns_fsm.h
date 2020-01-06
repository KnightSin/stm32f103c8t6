#ifndef NS_FSM_H
#define NS_FSM_H
#ifdef __cplusplus
extern "C" {
#endif
/*����״̬������̬ ��̬���¼�������̬ת��Ϊ��̬��ÿ��״̬����Ӧ��һ����������*/
//״̬�ռ� �¼��ռ�
extern void xprintf_s(const char*	fmt,...);
#define NULL 0
#define show_info(fmt,...)	xprintf_s(fmt,__VA_ARGS__)
#define tostring(str)	#str
//�¼�����
typedef int Fsm_Event_t;
//״̬����
typedef struct Fsm_State_t {
	int state_id;
	const char *state_name;
	void(*action)(void*);
	Fsm_Event_t transfer_event;//ת�Ƶ���̬Ҫ����¼�
	struct Fsm_State_t *next_state;
}Fsm_State_t;
//״̬������
typedef struct Fsm_t {
	Fsm_State_t *state;//״̬�ռ�
	Fsm_State_t *cur_state;//��ǰ״̬
	void(*set_cur_state)(struct Fsm_t *fsm, int id, void *action_param);//���õ�ǰ״̬��ִ��action
	Fsm_State_t *(*get_cur_state)(struct Fsm_t *fsm);//��ȡ��ǰ״̬��ָ��
	void(*input_event)(struct Fsm_t *fsm, Fsm_Event_t, void *);//��״̬������һ���¼���ִ��action
	void (*start)(struct Fsm_t *fsm, Fsm_State_t *start_state, Fsm_State_t *cur_state, void *action_param);
}Fsm_t;


void fsm_add_state(Fsm_t *fsm, Fsm_State_t *state);
void fsm_set_cur_state(Fsm_t *fsm, int id, void *action_param);
Fsm_State_t *fsm_get_cur_state(Fsm_t *fsm);
void fsm_input_event(Fsm_t *fsm, Fsm_Event_t event, void *action_param);
void fsm_start(Fsm_t *fsm, Fsm_State_t *start_state, Fsm_State_t *cur_state, void *action_param);
void fsm_test();
//�¼�����
typedef enum {
	high_vol = (0x01 << 0),
	low_vol = (0x01 << 1),
	unstable_high_lov = (0x01 << 2),
	unstable_low_lov = (0x01 << 3),
}Key_Event;
//״̬����
extern Fsm_State_t unstable_pressed;
extern Fsm_State_t unstable_released;
extern Fsm_State_t pressed;
extern Fsm_State_t released;
Fsm_State_t unstable_pressed = { 0,"unstable_pressed",NULL,low_vol, &pressed };
Fsm_State_t unstable_released = { 1,"unstable_released",NULL,high_vol,&released };
Fsm_State_t pressed = { 2,"pressed",NULL,unstable_high_lov,&unstable_released };
Fsm_State_t released = { 3,"released",NULL,unstable_low_lov,&unstable_pressed };
//״̬������
Fsm_t fsm_obj = {
	NULL,
	NULL,
	fsm_set_cur_state,
	fsm_get_cur_state,
	fsm_input_event,
	fsm_start,
};
Fsm_t *fsm = &fsm_obj;
#ifdef __cplusplus
}
#endif
#endif