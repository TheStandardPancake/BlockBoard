/**
*Copyright 2021 Boyd Kirkman
*
*This file is part of BlockBoard.
*
*    BlockBoard is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    BlockBoard is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with BlockBoard.  If not, see <https://www.gnu.org/licenses/>.
*/

//This is going to be the client interactive application for making a post and transfering funds.

#include <sha256.h>
/*
TODO:
-P2P connection network
-Block creating --> binding both posts and transactions
-Time stamp server
-Code formating features for posts
-mining system --> creating a tool for mining
-creating a tool for viewing posts
*/

void p2pConnect()
{

}

void login()
{

}

void guiLaunch();


int main()
{
  //Order of operations:

  //Connect to the P2P
  p2pConnect();

  //Login through provision of your private key so as to sign posts
  login();

  //Make transactions or make a post (a post has the inherent transaction of currency to even make the post)
  guiLaunch();


}
