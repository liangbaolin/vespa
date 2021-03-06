// Copyright 2017 Yahoo Holdings. Licensed under the terms of the Apache 2.0 license. See LICENSE in the project root.
package com.yahoo.vespa.curator;

import com.yahoo.path.Path;
import com.yahoo.vespa.curator.mock.MockCurator;
import org.junit.Test;

import java.time.Duration;

import static org.junit.Assert.fail;

/**
 * @author lulf
 */
public class CuratorCompletionWaiterTest {

    @Test
    public void testCompletionWaiter() throws InterruptedException {
        Curator curator = new MockCurator();
        Curator.CompletionWaiter waiter = CuratorCompletionWaiter.createAndInitialize(curator, Path.createRoot(), "foo", 1, "foo");
        Curator.CompletionWaiter notifier = CuratorCompletionWaiter.create(curator.framework(), Path.fromString("/foo"), 1, "bar");
        Thread t1 = new Thread(() -> {
            try {
                waiter.awaitCompletion(Duration.ofSeconds(120));
            } catch (CompletionTimeoutException e) {
                fail("Waiting failed due to timeout");
            }
        });
        notifier.notifyCompletion();
        t1.join();
    }

    @Test(expected = CompletionTimeoutException.class)
    public void testCompletionWaiterFailure() {
        Curator curator = new MockCurator();
        Curator.CompletionWaiter waiter = CuratorCompletionWaiter.createAndInitialize(curator, Path.createRoot(), "foo", 1, "foo");
        waiter.awaitCompletion(Duration.ofMillis(100));
    }
}
