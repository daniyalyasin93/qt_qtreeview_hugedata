/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>

#include "window.h"
#include <cstdlib>
#include <ctime>

Window::Window()
{
   m_pForm = new Ui::ListView();
   m_pForm->setupUi(this);
  
   m_pTreeView = m_pForm->treeView;

   m_pModel = new TreeModel(this);
   m_pTreeView->setModel(m_pModel);

   m_nMaxItemAmount = 1000;

   connect(&m_oDataProducer, SIGNAL(timeout()), this, SLOT(ProduceData()));

   QMetaObject::invokeMethod(this, "startProduction", Qt::QueuedConnection);
}

void Window::startProduction()
{
   m_oDataProducer.start(1);
}

// picked from: https://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
std::string gen_random(const int len) {
   static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";

   std::string str = "";
   str.resize(len);
   for (int i = 0; i < len; ++i) {
      str[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
   }

   return  str;
}

 
void Window::ProduceData()
{
   for (size_t i = 0; i < 500; i++)
   {

      tGUIData oGUIData;
      oGUIData.strCName = gen_random(10);
      oGUIData.strFName = gen_random(10);
      oGUIData.strMIDDec = gen_random(10);
      oGUIData.strMName = gen_random(10);



      tSEntry oSEntry;
      oSEntry.first = gen_random(10);
      oSEntry.second = gen_random(10);
      oGUIData.oSQueue.push_back(oSEntry);

      AddEntry(oGUIData);
   }
}
 
Window::~Window()
{
   m_oDataProducer.stop();
   if (m_pForm)
   {
      delete m_pForm;
      m_pForm = NULL;
   }
}

 
void Window::DeleteItems()
{
   int nRowCount = m_pModel->rowCount();
   if (nRowCount > m_nMaxItemAmount)
   {
      m_pModel->removeRows(0, nRowCount - m_nMaxItemAmount);
   }
}

 
void Window::AddEntry(tGUIData& oGUIData)
{
   DeleteItems();
   TaskItem* pMessageItem = new TaskItem();

   QStringList columns;
   columns << oGUIData.strCName.c_str() 
      << oGUIData.strMName.c_str()
      << oGUIData.strTimeStamp.c_str()
      << oGUIData.strPName.c_str();

   pMessageItem->addColumns(columns);

   while (!oGUIData.oSQueue.empty())
   {
      TaskItem* pSubItem = new TaskItem();
      QStringList sColumns;
      tSEntry& sEntry = oGUIData.oSQueue.front();
      sColumns << sEntry.first.c_str()
         << sEntry.second.c_str();

      pSubItem->addColumns(sColumns);
      pMessageItem->addChild(pSubItem);
      oGUIData.oSQueue.erase(oGUIData.oSQueue.begin());
   }
   m_pModel->appendRow(QModelIndex(), pMessageItem);
}
