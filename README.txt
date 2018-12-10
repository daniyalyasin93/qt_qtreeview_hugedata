I have made a QTreeView to display a very large and continuous data set. Since the data set is continuous, I delete initial rows when the total number of rows is greater than a specified amount.

I have used a custom model for this purpose

The whole system is working correctly and displaying data.

But I want it to autoscroll to the bottom to display latest data. If i use scrollToBottom at row addition it completely slows down the entire view-model. But If i use m_pTreeView->setAutoScroll at the start, it has not effect.

Moreover if i click on the view, it completely slows down.

I am using Qt 4.7.1

How should I auto scroll to the bottom without compromising on performance?
And show I remove the lag/drastic performance hit when I click on the view?
