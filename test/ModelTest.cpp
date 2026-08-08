// tests/ModelTests.cpp

#include <iostream>
#include <sstream>
#include <streambuf>
#include <string>
#include <vector>
#include <cstddef>
#include <ctime>

#include "../includes/models/BlobReference.h"
#include "../includes/models/Blob.h"
#include "../includes/models/Commit.h"
#include "../includes/datastructures/HashNode.h" // Note: avoid including .cpp files directly
#include "../includes/datastructures/HashTable.h"
#include "../includes/models/Index.h"
#include "../includes/models/HEAD.h"

namespace {
    int failures = 0;

    void printHeading(const std::string& title) {
        std::cout << "==================================\n";
        std::cout << "Testing " << title << "\n";
        std::cout << "==================================\n";
    }

    void check(bool condition, const std::string& testName) {
        if (condition) {
            std::cout << "PASS : " << testName << "\n";
        } else {
            std::cout << "FAIL : " << testName << "\n";
            ++failures;
        }
    }
}

void testBlobReference() {
    printHeading("BlobReference");

    BlobReference defaultReference;
    check(defaultReference.filename.empty(), "Default filename is empty");
    check(defaultReference.blobHash.empty(), "Default blobHash is empty");

    BlobReference reference{"src/main.cpp", "abc123"};
    check(reference.filename == "src/main.cpp", "filename is stored");
    check(reference.blobHash == "abc123", "blobHash is stored");
}

void testBlob() {
    printHeading("Blob");

    Blob defaultBlob;
    check(defaultBlob.getHash().empty(), "Default constructor initializes empty hash");
    check(defaultBlob.getFilename().empty(), "Default constructor initializes empty filename");
    check(defaultBlob.getContent().empty(), "Default constructor initializes empty content");
    check(defaultBlob.getSize() == 0, "Default constructor initializes size to zero");
    check(defaultBlob.getCreatedAt() == static_cast<std::time_t>(0), "Default constructor initializes createdAt to zero");

    Blob blob(
        "hash123",
        "file.txt",
        "Hello",
        5,
        static_cast<std::time_t>(1234567890)
    );

    check(blob.getHash() == "hash123", "getHash() returns constructor value");
    check(blob.getFilename() == "file.txt", "getFilename() returns constructor value");
    check(blob.getContent() == "Hello", "getContent() returns constructor value");
    check(blob.getSize() == 5, "getSize() returns constructor value");
    check(blob.getCreatedAt() == static_cast<std::time_t>(1234567890), "getCreatedAt() returns constructor value");

    const Blob& constBlob = blob;
    check(constBlob.getHash() == "hash123", "const getHash() works");
    check(constBlob.getFilename() == "file.txt", "const getFilename() works");
    check(constBlob.getContent() == "Hello", "const getContent() works");
    check(constBlob.getSize() == 5, "const getSize() works");
    check(constBlob.getCreatedAt() == static_cast<std::time_t>(1234567890), "const getCreatedAt() works");
}

void testCommit() {
    printHeading("Commit");

    Commit defaultCommit;
    check(defaultCommit.getCommitID().empty(), "Default constructor initializes empty commit ID");
    check(defaultCommit.getParentCommitID().empty(), "Default constructor initializes empty parent commit ID");
    check(defaultCommit.getBlobs().empty(), "Default constructor initializes empty blob list");
    check(defaultCommit.getMessage().empty(), "Default constructor initializes empty message");
    check(defaultCommit.getCreatedAt() == static_cast<std::time_t>(0), "Default constructor initializes createdAt to zero");

    BlobReference firstReference{"a.txt", "hashA"};
    BlobReference secondReference{"b.txt", "hashB"};

    std::vector<BlobReference> blobReferences;
    blobReferences.push_back(firstReference);
    blobReferences.push_back(secondReference);

    Commit commit(
        "commit123",
        "parent123",
        blobReferences,
        "Test commit",
        static_cast<std::time_t>(999)
    );

    check(commit.getCommitID() == "commit123", "getCommitID() returns constructor value");
    check(commit.getParentCommitID() == "parent123", "getParentCommitID() returns constructor value");
    check(commit.getMessage() == "Test commit", "getMessage() returns constructor value");
    check(commit.getCreatedAt() == static_cast<std::time_t>(999), "getCreatedAt() returns constructor value");
    check(commit.getBlobs().size() == 2, "getBlobs() returns correct blob count");

    if (commit.getBlobs().size() == 2) {
        check(commit.getBlobs()[0].filename == "a.txt", "First blob filename is stored");
        check(commit.getBlobs()[0].blobHash == "hashA", "First blob hash is stored");
        check(commit.getBlobs()[1].filename == "b.txt", "Second blob filename is stored");
        check(commit.getBlobs()[1].blobHash == "hashB", "Second blob hash is stored");
    } else {
        check(false, "Blob vector size is large enough for indexed checks");
    }

    const Commit& constCommit = commit;
    check(constCommit.getCommitID() == "commit123", "const getCommitID() works");
    check(constCommit.getParentCommitID() == "parent123", "const getParentCommitID() works");
    check(constCommit.getBlobs().size() == 2, "const getBlobs() works");
    check(constCommit.getMessage() == "Test commit", "const getMessage() works");
    check(constCommit.getCreatedAt() == static_cast<std::time_t>(999), "const getCreatedAt() works");
}

void testHashNode() {
    printHeading("HashNode");

    HashNode node;
    check(node.getFilename().empty(), "Default constructor initializes empty filename");
    check(node.getBlobHash().empty(), "Default constructor initializes empty blob hash");
    check(node.getNext() == nullptr, "Default constructor initializes next to nullptr");

    HashNode nodeWithValues("file.txt", "hash1");
    check(nodeWithValues.getFilename() == "file.txt", "Parameterized constructor stores filename");
    check(nodeWithValues.getBlobHash() == "hash1", "Parameterized constructor stores blob hash");
    check(nodeWithValues.getNext() == nullptr, "Parameterized constructor initializes next to nullptr");

    nodeWithValues.setBlobHash("hash2");
    check(nodeWithValues.getBlobHash() == "hash2", "setBlobHash() updates blob hash");

    HashNode nextNode("next.txt", "hashNext");
    nodeWithValues.setNext(&nextNode);

    HashNode* nextPointer = nodeWithValues.getNext();
    check(nextPointer == &nextNode, "setNext() updates next pointer");
    check(nextPointer != nullptr && nextPointer->getFilename() == "next.txt", "getNext() provides access to next node");
}

void testHashTable() {
    printHeading("HashTable");

    HashTable table;

    check(table.empty(), "empty() returns true initially");
    check(table.size() == 0, "size() returns zero initially");

    table.insert("a.txt", "hashA");
    check(table.size() == 1, "insert() adds a new file");
    check(table.contains("a.txt"), "contains() finds inserted file");
    check(table.getBlobHash("a.txt") == "hashA", "getBlobHash() returns inserted hash");

    table.insert("a.txt", "hashA");
    check(table.size() == 1, "insert() duplicate filename with same hash does not change size");
    check(table.getBlobHash("a.txt") == "hashA", "insert() duplicate filename with same hash keeps existing hash");

    table.insert("a.txt", "hashB");
    check(table.size() == 1, "insert() duplicate filename with different hash does not change size");
    check(table.getBlobHash("a.txt") == "hashB", "insert() duplicate filename with different hash updates hash");

    table.insert("b.txt", "hashC");
    check(table.size() == 2, "insert() adds a second file");

    check(!table.contains("missing.txt"), "contains() returns false for missing file");
    check(table.getBlobHash("missing.txt") == "", "getBlobHash() returns empty string for missing file");

    bool updatedExisting = table.update("b.txt", "hashCUpdated");
    check(updatedExisting, "update() returns true when file exists");
    check(table.getBlobHash("b.txt") == "hashCUpdated", "update() changes existing hash");

    bool updatedMissing = table.update("missing.txt", "missingHash");
    check(!updatedMissing, "update() returns false when file does not exist");

    bool removedExisting = table.remove("a.txt");
    check(removedExisting, "remove() returns true when file exists");
    check(!table.contains("a.txt"), "remove() removes existing file");
    check(table.size() == 1, "remove() decreases size");

    bool removedMissing = table.remove("missing.txt");
    check(!removedMissing, "remove() returns false when file does not exist");

    std::vector<BlobReference> references = table.getAllBlobReferences();
    check(references.size() == 1, "getAllBlobReferences() returns correct count before clear");
    check(
        references.size() == 1 && references[0].filename == "b.txt",
        "getAllBlobReferences() returns correct filename"
    );
    check(
        references.size() == 1 && references[0].blobHash == "hashCUpdated",
        "getAllBlobReferences() returns correct blob hash"
    );

    table.clear();
    check(table.empty(), "clear() makes table empty");
    check(table.size() == 0, "clear() sets size to zero");
    check(!table.contains("b.txt"), "clear() removes all files");

    HashTable displayTable;
    displayTable.insert("display.txt", "displayHash");

    {
        std::ostringstream suppressedOutput;
        std::streambuf* originalBuffer = std::cout.rdbuf(suppressedOutput.rdbuf());
        displayTable.display();
        std::cout.rdbuf(originalBuffer);
    }

    check(true, "display() executes");

    HashTable collisionTable;
    const std::size_t fileCount = 200;

    for (std::size_t i = 0; i < fileCount; ++i) {
        std::string filename = "file_" + std::to_string(i) + ".txt";
        std::string blobHash = "hash_" + std::to_string(i);
        collisionTable.insert(filename, blobHash);
    }

    check(collisionTable.size() == fileCount, "Collision handling stores all inserted files");

    bool allFound = true;
    bool allHashesCorrect = true;

    for (std::size_t i = 0; i < fileCount; ++i) {
        std::string filename = "file_" + std::to_string(i) + ".txt";
        std::string blobHash = "hash_" + std::to_string(i);

        if (!collisionTable.contains(filename)) {
            allFound = false;
        }

        if (collisionTable.getBlobHash(filename) != blobHash) {
            allHashesCorrect = false;
        }
    }

    check(allFound, "Collision handling preserves contains() for all inserted files");
    check(allHashesCorrect, "Collision handling preserves blob hashes for all inserted files");

    std::vector<BlobReference> allReferences = collisionTable.getAllBlobReferences();
    check(allReferences.size() == fileCount, "getAllBlobReferences() returns all entries after collisions");
}

void testIndex() {
    printHeading("Index");

    Index index;

    check(index.getStagedFiles().empty(), "Index starts empty");
    check(index.getStagedFiles().size() == 0, "Index starts with size zero");

    index.getStagedFiles().insert("staged1.txt", "hash1");
    index.getStagedFiles().insert("staged2.txt", "hash2");

    check(index.getStagedFiles().size() == 2, "Staged files can be inserted through getStagedFiles()");
    check(index.getStagedFiles().contains("staged1.txt"), "Staged file one is contained");
    check(index.getStagedFiles().contains("staged2.txt"), "Staged file two is contained");

    const Index& constIndex = index;
    check(constIndex.getStagedFiles().contains("staged1.txt"), "const getStagedFiles() provides read access");
    check(constIndex.getStagedFiles().size() == 2, "const getStagedFiles() returns correct size");

    index.getStagedFiles().clear();

    check(index.getStagedFiles().empty(), "Staged files can be cleared through getStagedFiles()");
    check(index.getStagedFiles().size() == 0, "Staged file size is zero after clear");
}

void testHEAD() {
    printHeading("HEAD");

    HEAD defaultHead;
    check(defaultHead.getCurrentCommitID().empty(), "Default constructor initializes empty current commit ID");

    HEAD headWithCommit("commitA");
    check(headWithCommit.getCurrentCommitID() == "commitA", "Parameterized constructor stores current commit ID");

    headWithCommit.setCurrentCommitID("commitB");
    check(headWithCommit.getCurrentCommitID() == "commitB", "setCurrentCommitID() updates current commit ID");

    const HEAD& constHead = headWithCommit;
    check(constHead.getCurrentCommitID() == "commitB", "const getCurrentCommitID() works");
}

int main() {
    testBlobReference();
    testBlob();
    testCommit();
    testHashNode();
    testHashTable();
    testIndex();
    testHEAD();

    std::cout << "==================================\n";

    if (failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
    } else {
        std::cout << failures << " TEST(S) FAILED\n";
    }

    std::cout << "==================================\n";

    return failures == 0 ? 0 : 1;
}