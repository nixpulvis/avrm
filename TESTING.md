# AVR Automated Testing

Testing on hardware can be challenging, especially when more than one device are needed to cooperate. This demands a different approach to testing. Whereas in some cases it's enough to simply have a single test program, there are many cases that is simply not possible. Having a collection of tests to run against code is critical to "sane" software development. The goal of this document is to allow us to do that within the world of embedded software development.

Important differences with traditional automated testing include:

- Program size restrictions.
- Cooperating hardware tests.
- Program to user communication.
- Real world influences.

Each of these brings on their own set of challenges and solutions. In some cases the solutions are not software solutions, RF range tests for example cannot be automatically tested without complex robotics, or preset devices at varying ranges. This document aims to solve the first 3 issues listed.

## Test Suite

A test suite is a folder containing test and other test suites. Running a test suite is simply defined to be running every test and test suite in the suite. Likewise, a test suite is defined to be passing if all of it's tests pass. The name of a test suite is it's folder name.

Since hardware state is a factor in some programs, running of test suites should be randomized with a seed. The seed should be reported as part of the test results to allow reproduction.

## Test

A test is a single program. Unlike the way things are done in other frameworks, a test program should only test one thing. This means there is a decent bit of repeated setup code, but it ensures the test programs will operate in isolation. It also solves the program size problem, since a single test should never test more than one piece of functionality.

A test can be uploaded to more than one device by adding a special test directive in a comment anywhere in the test. This allows tests to be run on more than one device at the same time. RF communication tests are a great example of where this is needed. The test program can get its device number from the `TEST_DEVICE` constant.

```c
//!test[devices=3]

int main(void)
    switch (TEST_DEVICE)
    {
        case 0:
            write_data();
            break;
        case 1:
            read_data();
            break;
        case 2:
            generate_noise();
            break;
    }

    return 0;
}

```

A test program like this will be uploaded to 3 devices. The 0th device will call `write_data()`, the 1st device will call `read_data()`, and finally the 2nd device will call `generate_noise()`.

You can ask for the status of another test device too. This allows the previous example to be more robust against variances in upload timing. What we really want to do is wait for the noise, then write only when reading. This order can be achieved by using test checkpoints.

```c
//!test[devices=3]

int main(void)
    if (TEST_DEVICE == 0 && TEST_CHECKPOINT_GET(1, "reading"))
    {
        int ret = write_data();

        if (ret == 0)
            TEST_PASS();
        else
            TEST_FAIL();
    }

    if (TEST_DEVICE == 1 && TEST_CHECKPOINT(2, "active"))
    {
        byte *data = read_data();
        TEST_CHECKPOINT_SET("reading");

        if (data == "foobar")
            TEST_PASS();
        else
            TEST_FAIL();
    }

    if (TEST_DEVICE == 2)
        generate_noise();
        TEST_CHECKPOINT_SET("active");
        TEST_PASS();
    }

    return 0;
}

```

The communication between the devices is handled over UART, so it's important to keep that in mind when thinking through the timing of the tests.

## Tester

The tool that manages all of the running of a test suite and individual tests is the tester, named `avr-test`.

```bash
avr-test [test suite | test program]
```

```bash
avr-test rf_send_one_byte_test
[TESTER] Compiling...done
[TESTER] Flashing to 3 devices...done
[DEVICE_2:CHECKPOINT] active
[DEVICE_2:TEST] PASS
[DEVICE_1:CHECKPOINT] reading
[DEVICE_0:TEST] PASS
[DEVICE_1:TEST] PASS
```