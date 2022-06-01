#include "MsgPubSub.h"

void Sub::update(Msg *m) 
{
  Msg *m_copy = new Msg(m);
  mtx.lock();
  que.push(m_copy);
  cv.notify_all();
  mtx.unlock();
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

	  if (msg->what == MSG_END) {
      console_print(name + ".run() terminated.\n");
	    break;
    }

    handle(msg);
  
    delete msg; 
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

