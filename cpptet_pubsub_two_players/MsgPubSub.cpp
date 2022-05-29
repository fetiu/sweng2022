#include "MsgPubSub.h"

void Sub::update(Msg *m) 
{
#if 1 // added by khkim
  Msg *m_copy = new Msg(m);
  mtx.lock();
  que.push(m_copy);
  cv.notify_all();
  mtx.unlock();
#else
  mtx.lock();
  que.push(m);
  cv.notify_all();
  mtx.unlock();
#endif
}

void Sub::run() 
{
  //win->printw(name + ".run() begins.\n");
  while (true) {
    unique_lock<mutex> uqlck(mtx);
    if (que.empty())
      cv.wait(uqlck, [&]{ return !que.empty(); } );
    
    Msg *msg = que.front();
    que.pop();
    uqlck.unlock();
    //win->printw(name + ".run() wakes up.\n");

	  if (msg->what == MSG_END)
	    break;

    handle(msg);
  
#if 1 // added by khkim
    delete msg; 
#endif
  }
  //win->printw(name + ".run() ends.\n");
};

void Pub::addSubs(Sub* sub) 
{
  subs[nsubs] = sub;
  nsubs++;
}
  
void Pub::notifySubs(Msg *m) 
{
  for(int i=0; i < nsubs; i++)
    subs[i]->update(m);
}

