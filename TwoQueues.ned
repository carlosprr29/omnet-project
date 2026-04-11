//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

import ned.IdealChannel;
import src.Queue;
import src.Server;
import src.ServerWithMaintenance;
import src.Sink;
import src.Source;


network TwoQueues
{
    @display("bgb=373,202");
    submodules:
        serverWithMaintenance: ServerWithMaintenance {
            @display("p=243,139");
         }
        source2: Source {
            @display("p=29,139");
        }
        source1: Source {
            @display("p=29,35");
        }
        queue2: Queue {
            @display("p=124,139");
        }
        queue1: Queue {
            @display("p=124,35");
        }
        server: Server {
            @display("p=243,35;i=block/server");
        }
        sink: Sink {
            @display("p=333,91");
        }
    connections:
        source1.out --> IdealChannel --> queue1.in;
        queue1.out --> IdealChannel --> server.in;
        source2.out --> IdealChannel --> queue2.in;
        queue2.out --> IdealChannel --> serverWithMaintenance.in;
        server.out --> IdealChannel --> sink.in++;
        serverWithMaintenance.out --> IdealChannel --> sink.in++;
}
