/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2014 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#include "ZipArchiveTests.h"
#include "Threading/OgreThreadHeaders.h"
#include "OgreZip.h"
#include "OgreCommon.h"


#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include "macUtils.h"
#endif

using namespace Ogre;

// Register the test suite ZipArchiveTests );

//--------------------------------------------------------------------------
void ZipArchiveTests::SetUp()
{
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    mTestPath = macBundlePath() + "/Contents/Resources/Media/misc/ArchiveTest.zip";
#elif OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    mTestPath = "../../Tests/OgreMain/misc/ArchiveTest.zip";
#else
    mTestPath = "./Tests/OgreMain/misc/ArchiveTest.zip";
#endif
}
//--------------------------------------------------------------------------
void ZipArchiveTests::TearDown()
{
}
//--------------------------------------------------------------------------
TEST_F(ZipArchiveTests,ListNonRecursive)
{
    ZipArchive* arch = OGRE_NEW ZipArchive(mTestPath, "Zip");
    try {
        arch->load();
    } catch (Ogre::Exception e) {
        // If it starts in build/bin/debug
        OGRE_DELETE arch;
        arch = OGRE_NEW ZipArchive("../../../" + mTestPath, "Zip");
        arch->load();
    }
    StringVectorPtr vec = arch->list(false);

    EXPECT_EQ((size_t)2, vec->size());
    EXPECT_EQ(String("rootfile.txt"), vec->at(0));
    EXPECT_EQ(String("rootfile2.txt"), vec->at(1));

    OGRE_DELETE arch;
}
//--------------------------------------------------------------------------
TEST_F(ZipArchiveTests,ListRecursive)
{
    ZipArchive* arch = OGRE_NEW ZipArchive(mTestPath, "Zip");
    try {
        arch->load();
    } catch (Ogre::Exception e) {
        // If it starts in build/bin/debug
        OGRE_DELETE arch;
        arch = OGRE_NEW ZipArchive("../../../" + mTestPath, "Zip");
        arch->load();
    }
    StringVectorPtr vec = arch->list(true);

    EXPECT_EQ((size_t)6, vec->size());
    EXPECT_EQ(String("file.material"), vec->at(0));
    EXPECT_EQ(String("file2.material"), vec->at(1));
    EXPECT_EQ(String("file3.material"), vec->at(2));
    EXPECT_EQ(String("file4.material"), vec->at(3));
    EXPECT_EQ(String("rootfile.txt"), vec->at(4));
    EXPECT_EQ(String("rootfile2.txt"), vec->at(5));

    OGRE_DELETE arch;
}
//--------------------------------------------------------------------------
TEST_F(ZipArchiveTests,ListFileInfoNonRecursive)
{
    ZipArchive* arch = OGRE_NEW ZipArchive(mTestPath, "Zip");
    try {
        arch->load();
    } catch (Ogre::Exception e) {
        // If it starts in build/bin/debug
        OGRE_DELETE arch;
        arch = OGRE_NEW ZipArchive("../../../" + mTestPath, "Zip");
        arch->load();
    }
    FileInfoListPtr vec = arch->listFileInfo(false);

    EXPECT_EQ((size_t)2, vec->size());
    FileInfo& fi1 = vec->at(0);
    EXPECT_EQ(String("rootfile.txt"), fi1.filename);
    EXPECT_EQ(BLANKSTRING, fi1.path);
    EXPECT_EQ((size_t)40, fi1.compressedSize);
    EXPECT_EQ((size_t)130, fi1.uncompressedSize);

    FileInfo& fi2 = vec->at(1);
    EXPECT_EQ(String("rootfile2.txt"), fi2.filename);
    EXPECT_EQ(BLANKSTRING, fi2.path);
    EXPECT_EQ((size_t)45, fi2.compressedSize);
    EXPECT_EQ((size_t)156, fi2.uncompressedSize);

    OGRE_DELETE arch;
}
//--------------------------------------------------------------------------
TEST_F(ZipArchiveTests,ListFileInfoRecursive)
{
    ZipArchive* arch = OGRE_NEW ZipArchive(mTestPath, "Zip");
    try {
        arch->load();
    } catch (Ogre::Exception e) {
        // If it starts in build/bin/debug
        OGRE_DELETE arch;
        arch = OGRE_NEW ZipArchive("../../../" + mTestPath, "Zip");
        arch->load();
    }
    FileInfoListPtr vec = arch->listFileInfo(true);

    EXPECT_EQ((size_t)6, vec->size());
    FileInfo& fi3 = vec->at(0);
    EXPECT_EQ(String("file.material"), fi3.filename);
    EXPECT_EQ(String("level1/materials/scripts/"), fi3.path);
    EXPECT_EQ((size_t)0, fi3.compressedSize);
    EXPECT_EQ((size_t)0, fi3.uncompressedSize);

    FileInfo& fi4 = vec->at(1);
    EXPECT_EQ(String("file2.material"), fi4.filename);
    EXPECT_EQ(String("level1/materials/scripts/"), fi4.path);
    EXPECT_EQ((size_t)0, fi4.compressedSize);
    EXPECT_EQ((size_t)0, fi4.uncompressedSize);

    FileInfo& fi5 = vec->at(2);
    EXPECT_EQ(String("file3.material"), fi5.filename);
    EXPECT_EQ(String("level2/materials/scripts/"), fi5.path);
    EXPECT_EQ((size_t)0, fi5.compressedSize);
    EXPECT_EQ((size_t)0, fi5.uncompressedSize);

    FileInfo& fi6 = vec->at(3);
    EXPECT_EQ(String("file4.material"), fi6.filename);
    EXPECT_EQ(String("level2/materials/scripts/"), fi6.path);
    EXPECT_EQ((size_t)0, fi6.compressedSize);
    EXPECT_EQ((size_t)0, fi6.uncompressedSize);

    FileInfo& fi1 = vec->at(4);
    EXPECT_EQ(String("rootfile.txt"), fi1.filename);
    EXPECT_EQ(BLANKSTRING, fi1.path);
    EXPECT_EQ((size_t)40, fi1.compressedSize);
    EXPECT_EQ((size_t)130, fi1.uncompressedSize);

    FileInfo& fi2 = vec->at(5);
    EXPECT_EQ(String("rootfile2.txt"), fi2.filename);
    EXPECT_EQ(BLANKSTRING, fi2.path);
    EXPECT_EQ((size_t)45, fi2.compressedSize);
    EXPECT_EQ((size_t)156, fi2.uncompressedSize);

    OGRE_DELETE arch;
}
//--------------------------------------------------------------------------
TEST_F(ZipArchiveTests,FindNonRecursive)
{
    ZipArchive* arch = OGRE_NEW ZipArchive(mTestPath, "Zip");
    try {
        arch->load();
    } catch (Ogre::Exception e) {
        // If it starts in build/bin/debug
        OGRE_DELETE arch;
        arch = OGRE_NEW ZipArchive("../../../" + mTestPath, "Zip");
        arch->load();
    }

    StringVectorPtr vec = arch->find("*.txt", false);

    EXPECT_EQ((size_t)2, vec->size());
    EXPECT_EQ(String("rootfile.txt"), vec->at(0));
    EXPECT_EQ(String("rootfile2.txt"), vec->at(1));

    OGRE_DELETE arch;
}
//--------------------------------------------------------------------------
TEST_F(ZipArchiveTests,FindRecursive)
{
    ZipArchive* arch = OGRE_NEW ZipArchive(mTestPath, "Zip");
    try {
        arch->load();
    } catch (Ogre::Exception e) {
        // If it starts in build/bin/debug
        OGRE_DELETE arch;
        arch = OGRE_NEW ZipArchive("../../../" + mTestPath, "Zip");
        arch->load();
    }

    StringVectorPtr vec = arch->find("*.material", true);

    EXPECT_EQ((size_t)4, vec->size());
    EXPECT_EQ(String("file.material"), vec->at(0));
    EXPECT_EQ(String("file2.material"), vec->at(1));
    EXPECT_EQ(String("file3.material"), vec->at(2));
    EXPECT_EQ(String("file4.material"), vec->at(3));

    OGRE_DELETE arch;
}
//--------------------------------------------------------------------------
TEST_F(ZipArchiveTests,FindFileInfoNonRecursive)
{
    ZipArchive* arch = OGRE_NEW ZipArchive(mTestPath, "Zip");
    try {
        arch->load();
    } catch (Ogre::Exception e) {
        // If it starts in build/bin/debug
        OGRE_DELETE arch;
        arch = OGRE_NEW ZipArchive("../../../" + mTestPath, "Zip");
        arch->load();
    }
    FileInfoListPtr vec = arch->findFileInfo("*.txt", false);

    EXPECT_EQ((size_t)2, vec->size());
    FileInfo& fi1 = vec->at(0);
    EXPECT_EQ(String("rootfile.txt"), fi1.filename);
    EXPECT_EQ(BLANKSTRING, fi1.path);
    EXPECT_EQ((size_t)40, fi1.compressedSize);
    EXPECT_EQ((size_t)130, fi1.uncompressedSize);

    FileInfo& fi2 = vec->at(1);
    EXPECT_EQ(String("rootfile2.txt"), fi2.filename);
    EXPECT_EQ(BLANKSTRING, fi2.path);
    EXPECT_EQ((size_t)45, fi2.compressedSize);
    EXPECT_EQ((size_t)156, fi2.uncompressedSize);

    OGRE_DELETE arch;
}
//--------------------------------------------------------------------------
TEST_F(ZipArchiveTests,FindFileInfoRecursive)
{
    ZipArchive* arch = OGRE_NEW ZipArchive(mTestPath, "Zip");
    try {
        arch->load();
    } catch (Ogre::Exception e) {
        // If it starts in build/bin/debug
        OGRE_DELETE arch;
        arch = OGRE_NEW ZipArchive("../../../" + mTestPath, "Zip");
        arch->load();
    }
    FileInfoListPtr vec = arch->findFileInfo("*.material", true);

    EXPECT_EQ((size_t)4, vec->size());

    FileInfo& fi3 = vec->at(0);
    EXPECT_EQ(String("file.material"), fi3.filename);
    EXPECT_EQ(String("level1/materials/scripts/"), fi3.path);
    EXPECT_EQ((size_t)0, fi3.compressedSize);
    EXPECT_EQ((size_t)0, fi3.uncompressedSize);

    FileInfo& fi4 = vec->at(1);
    EXPECT_EQ(String("file2.material"), fi4.filename);
    EXPECT_EQ(String("level1/materials/scripts/"), fi4.path);
    EXPECT_EQ((size_t)0, fi4.compressedSize);
    EXPECT_EQ((size_t)0, fi4.uncompressedSize);

    FileInfo& fi5 = vec->at(2);
    EXPECT_EQ(String("file3.material"), fi5.filename);
    EXPECT_EQ(String("level2/materials/scripts/"), fi5.path);
    EXPECT_EQ((size_t)0, fi5.compressedSize);
    EXPECT_EQ((size_t)0, fi5.uncompressedSize);

    FileInfo& fi6 = vec->at(3);
    EXPECT_EQ(String("file4.material"), fi6.filename);
    EXPECT_EQ(String("level2/materials/scripts/"), fi6.path);
    EXPECT_EQ((size_t)0, fi6.compressedSize);
    EXPECT_EQ((size_t)0, fi6.uncompressedSize);

    OGRE_DELETE arch;
}
//--------------------------------------------------------------------------
TEST_F(ZipArchiveTests,FileRead)
{
    ZipArchive* arch = OGRE_NEW ZipArchive(mTestPath, "Zip");
    try {
        arch->load();
    } catch (Ogre::Exception e) {
        // If it starts in build/bin/debug
        OGRE_DELETE arch;
        arch = OGRE_NEW ZipArchive("../../../" + mTestPath, "Zip");
        arch->load();
    }

    DataStreamPtr stream = arch->open("rootfile.txt");
    EXPECT_EQ(String("this is line 1 in file 1"), stream->getLine());
    EXPECT_EQ(String("this is line 2 in file 1"), stream->getLine());
    EXPECT_EQ(String("this is line 3 in file 1"), stream->getLine());
    EXPECT_EQ(String("this is line 4 in file 1"), stream->getLine());
    EXPECT_EQ(String("this is line 5 in file 1"), stream->getLine());
    EXPECT_TRUE(stream->eof());

    OGRE_DELETE arch;
}
//--------------------------------------------------------------------------
TEST_F(ZipArchiveTests,ReadInterleave)
{
    // Test overlapping reads from same archive
    ZipArchive* arch = OGRE_NEW ZipArchive(mTestPath, "Zip");
    try {
        arch->load();
    } catch (Ogre::Exception e) {
        // If it starts in build/bin/debug
        OGRE_DELETE arch;
        arch = OGRE_NEW ZipArchive("../../../" + mTestPath, "Zip");
        arch->load();
    }

    // File 1
    DataStreamPtr stream1 = arch->open("rootfile.txt");
    EXPECT_EQ(String("this is line 1 in file 1"), stream1->getLine());
    EXPECT_EQ(String("this is line 2 in file 1"), stream1->getLine());

    // File 2
    DataStreamPtr stream2 = arch->open("rootfile2.txt");
    EXPECT_EQ(String("this is line 1 in file 2"), stream2->getLine());
    EXPECT_EQ(String("this is line 2 in file 2"), stream2->getLine());
    EXPECT_EQ(String("this is line 3 in file 2"), stream2->getLine());

    // File 1
    EXPECT_EQ(String("this is line 3 in file 1"), stream1->getLine());
    EXPECT_EQ(String("this is line 4 in file 1"), stream1->getLine());
    EXPECT_EQ(String("this is line 5 in file 1"), stream1->getLine());
    EXPECT_TRUE(stream1->eof());

    // File 2
    EXPECT_EQ(String("this is line 4 in file 2"), stream2->getLine());
    EXPECT_EQ(String("this is line 5 in file 2"), stream2->getLine());
    EXPECT_EQ(String("this is line 6 in file 2"), stream2->getLine());
    EXPECT_TRUE(stream2->eof());

    OGRE_DELETE arch;
}
//--------------------------------------------------------------------------
