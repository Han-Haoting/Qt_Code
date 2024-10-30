#include "Injection_Image_Full.h"

void Injection_Image_Full()
{
    extern Mat detection_result,detection_result_RGB;
    cvtColor(detection_result_RGB,detection_result,COLOR_BGR2GRAY);
    Mat fcop;
    detection_result.copyTo(fcop);
    vector<Point> upp;//定义四连通点集，有必要可以是八连通
    upp.push_back(Point(-1, 0));
    upp.push_back(Point(0, -1));
    upp.push_back(Point(0, 1));
    upp.push_back(Point(1, 0));
    //upp.push_back(Point(1, 1));
    //upp.push_back(Point(-1,-1));
    //upp.push_back(Point(-1, 1));
    //upp.push_back(Point(1, -1));

    vector<vector<vector<Point>>> lenm;//三维point向量 lenm.size()是连通域的个数
    /*
        int impixel_sum = 0;
        for (int j = 0,j<lenm[i].size();j++)
        {
            impixel_sum+= lenm[i][j].size();
        }
        //这段循环表示第i个连通域中像素点的个数。
    */
    vector<vector<Point>> numim;
    vector<Point> ssinum;
    vector<vector<int>> Flag;
    vector<int> ce;

    int nmss = 0;//连通域的个数；
    int nums = 0;//中间变量用来存储 lenm.size();即在程序运行过程中nums始终等于lenm[i][j][k]中的j的值的大小;
    int s1 = 0;
    //标志位 ，每次区域生长后 符合条件的像素个数，当第i个连通域，在经过第j次生长后，s1=lenm[i][j].size(),
    //若s1==0,表示生长结束,不再有符合条件的点，第i连通域中的所有点都已经找到。

    for (int row = 0; row < fcop.rows; row++)
    {
        for (int col = 0; col < fcop.cols; col++)
        {
            if (fcop.at<uchar>(row, col) == 0)
            {
                ce.push_back(1);
                Flag.push_back(ce);
                //vector<vector<Point>> numim;
                //vector<Point> ssinum;
                ssinum.push_back(Point(col, row));
                numim.push_back(ssinum);
                fcop.at<uchar>(row, col) = 255;
                ssinum.clear();
                s1 = 1;
                while (s1 > 0)
                {
                    //ce.push_back(1);
                    //Flag.push_back(ce);
                    //vector<Point> ssinum;
                    for (unsigned long long i = 0; i < numim[nums].size(); i++)
                    {
                        for (unsigned long long j = 0; j < upp.size(); j++)
                        {
                            int X = numim[nums][i].x + upp[j].x;
                            int Y = numim[nums][i].y + upp[j].y;
                            if (X >= 0 && Y >= 0 && X < fcop.cols && Y < fcop.rows)
                            {
                                if (fcop.at<uchar>(Y, X) == 0)
                                {
                                    ssinum.push_back(Point(X, Y));
                                    fcop.at<uchar>(Y, X) = 255;
                                }
                            }

                            if (X == 0 || Y == 0 || X == fcop.cols - 1 || Y == fcop.rows - 1)
                            {
                                Flag[nmss][0] = 0;

                            }
                        }
                    }
                    //Flag.push_back(ce);
                    numim.push_back(ssinum);
                    s1 = ssinum.size();
                    nums++;
                    ssinum.clear();
                    /*ce.clear();*/
                }
                nums = 0;
                lenm.push_back(numim);
                numim.clear();
                nmss++;
                ce.clear();
            }

        }
    }
    //imshow("1",fcop);
    //ffcop = Mat::zeros(cop.size(),cop.type());
    for (unsigned long long i = 0; i < Flag.size(); i++)
    {
        if (Flag[i][0] == 1)
        {
            for (unsigned long long j = 0; j < lenm[i].size(); j++)
            {
                for (unsigned long long k = 0; k < lenm[i][j].size(); k++)
                {
                    int X = lenm[i][j][k].x;
                    int Y = lenm[i][j][k].y;
                    detection_result.at<uchar>(Y, X) = 255;
                }
            }
        }
    }

}
