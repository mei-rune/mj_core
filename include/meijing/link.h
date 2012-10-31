#ifndef _mj_link_h_
#define _mj_link_h_ 1

// SLINK - ��������
//
//typedef struct _SLINK {
//   _SLINK* _next;
//} SLINK,*PSLINK;


//��ʼ����������
//template< typename PSLINK >
//inline void SLINK_Initialize(PSLINK _head ) { (_head)->_next = NULL; }
#define SLINK_Initialize(_head)           ((_head)->_next = NULL)

//��ⵥ�������Ƿ�Ϊ��
//template< typename PSLINK >
//inline bool SLINK_IsEmpty(PSLINK _head )
//{ return ((_head)->_next == NULL);}
#define SLINK_IsEmpty(_head)              ((_head)->_next == NULL)

//ȡ�����������һ����Ŀ
//template< typename PSLINK ,typename PVALUE >
//inline PVALUE SLINK_Pop(PSLINK _head ) {
//  PVALUE head_item = (_head)->_next;
//  (_head)->_next =  ((_head)->_next->_next);
//  return head_item;
//}
#define SLINK_Pop(_head)                  (_head)->_next;\
                                          (_head)->_next =  (_head)->_next->_next;

//����Ŀ���뵥������ͷ��
//template< typename PSLINK ,typename PVALUE >
//inline void SLINK_Push(PSLINK  _head, PVALUE  _link) {
// (_link)->_next =  (_head)->_next;
// (_head)->_next =  (_link);
//}
#define SLINK_Push(_head, _link)          (_link)->_next =  (_head)->_next; \
                                          (_head)->_next =  (_link)

///////////////////////////////////////////////////////////////////////////////////
/////////////////////////////DOUBLE///LINK/////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

//DLINK - ˫������
//
//typedef struct _DLINK {
//   _DLINK* _prev;
//   _DLINK* _next;
//} DLINK,*PDLINK;

//��ʼ��˫������
//template< typename PDLINK >
//void DLINK_Initialize(PDLINK  _head) {
// (_head)->_next = (_head)->_prev = (_head);
//}
#define DLINK_Initialize(_head)            ((_head)->_next = (_head)->_prev = (_head))

//���˫�������Ƿ�Ϊ��
//template< typename PDLINK >
//bool DLINK_IsEmpty(PDLINK) {
//  return ((_head)->_next == (_head));
//}
#define DLINK_IsEmpty(_head)               ((_head)->_next == (_head))

//����Ŀ����˫������ͷ��֮��
//template< typename PDLINK ,typename PVALUE >
//inline void DLINK_InsertNext(PDLINK _head,PVALUE _el) {
//	(_el)->_next = (_head)->_next;
//  (_el)->_prev = (_head);
//  (_head)->_next->_prev = (_el);
//  (_head)->_next = (_el);
//}
#define DLINK_InsertNext(_head, _el)     (_el)->_next = (_head)->_next;\
                                           (_el)->_prev = (_head);\
                                           (_head)->_next->_prev = (_el);\
                                           (_head)->_next = (_el)

//����Ŀ����˫������ͷ��֮ǰ
//template< typename PDLINK ,typename PVALUE >
//inline void DLINK_InsertPrev(PDLINK _head, PVALUE _el) {
//  (_el)->_prev = (_head)->_prev;
//  (_el)->_next = (_head);
//  (_head)->_prev->_next = (_el);
//  (_head)->_prev = (_el);
//}
#define DLINK_InsertPrev(_head, _el)     (_el)->_prev = (_head)->_prev;\
                                           (_el)->_next = (_head);\
                                           (_head)->_prev->_next = (_el);\
                                           (_head)->_prev = (_el)
//��˫��������ɾ����ǰ��Ŀ
//template< typename PDLINK >
//inline void DLINK_Remove(PDLINK _el) {
//	(_el)->_prev->_next = (_el)->_next;
//  (_el)->_next->_prev = (_el)->_prev;
//}
#define DLINK_Remove(_el)               (_el)->_prev->_next = (_el)->_next;\
                                           (_el)->_next->_prev = (_el)->_prev
//��˫��������ȡ����ǰ��Ŀ��ǰһ��
//template< typename PDLINK ,typename PVALUE >
//inline PVALUE DLINK_ExtructPrev(PDLINK _head ) {
//	PVALUE v = (_head)->_prev;
//  DLINK_Remove((_head)->_prev);
//	return v;
//}
#define DLINK_ExtructPrev(_head)           (_head)->_prev;\
                                          DLINK_Remove((_head)->_prev)
//��˫��������ȡ����ǰ��Ŀ����һ��
//template< typename PDLINK ,typename PVALUE >
//inline PVALUE DLINK_ExtructNext(PDLINK _head) {
//	PVALUE v = (_head)->_next;
//  DLINK_Remove((_head)->_next);
//	return v;
//}
#define DLINK_ExtructNext(_head)           (_head)->_next;\
                                           DLINK_Remove((_head)->_next)


#endif // _link_h_
