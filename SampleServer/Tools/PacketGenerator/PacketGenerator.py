

import argparse
import jinja2
import ProtoParser

def main():
    arg_parser = argparse.ArgumentParser(description = 'PacketGenerator')
    arg_parser.add_argument('--path', type=str, default='C:/Users/user/Documents/GitHub/52_Project/SampleServer/Common/Protobuf/bin/Protocol.proto', help='proto path')
    arg_parser.add_argument('--output', type=str, default='TestPacketHandler', help='output file')
    arg_parser.add_argument('--recv', type=str, default='C_', help='recv convention')
    arg_parser.add_argument('--send', type=str, default='S_', help='send convention')
    args = arg_parser.parse_args()

    parser = ProtoParser.ProtoParser(1000, args.recv, args.send)
    parser.parse_proto(args.path)
    
    # jinja2
    # 폴더 찾기
    file_loader = jinja2.FileSystemLoader('Templates')
    env = jinja2.Environment(loader=file_loader)

    template = env.get_template('PacketHandler.h')
    # 얘가 진짜 작업하는 애임.
    # 우리가 안에서 사용할 데이터들을 매핑
    output = template.render(parser=parser, output=args.output)

    f = open(args.output+'.h', 'w+')
    f.write(output)
    f.close()

    print(output)
    return

if __name__ == '__main__':
    main()
    